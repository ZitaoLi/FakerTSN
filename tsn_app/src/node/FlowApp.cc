#include "FlowApp.h"

#include "FlowTicker.h"

namespace faker_tsn
{

FlowApp::FlowApp(const char* deviceName) : m_deviceName(deviceName), flowIndex(0), flowNum(0), frameCount(0), lastFrameSize(MAX_FRAME_SIZE) {
    INFO(this->toString() + " construct FlowApp");
    /* load xml */
    INFO(this->toString() +  " load flows.xml");
    std::string filename = "./config/flows.xml";
    this->loadScheduleXML(filename);
}

FlowApp::~FlowApp() {
    INFO(this->toString() + " desstruct FlowApp");
    shutdown(this->m_iSockfd, SHUT_RDWR);
    shutdown(this->m_oSockfd, SHUT_RDWR);
    close(this->m_iSockfd);
    close(this->m_oSockfd);
}

bool _compare(const Flow* f1, const Flow* f2) {
    return f1->startOffset >= f2->startOffset;
}

void FlowApp::loadScheduleXML(std::string filename) {
    /* init root element */
    XMLDocument doc;
    doc.LoadFile(filename.c_str());
    XMLElement* schedule = doc.RootElement();
    /* get size unit */
    const char* sizeUnit = schedule->Attribute("sizeUnit");
    INFO("size unit: \t" + std::string(sizeUnit));
    /* get time unit */
    const char* timeUnit = schedule->Attribute("timeUnit");
    INFO("time unit: \t" + std::string(timeUnit));
    /* get hyper period */
    long hyperPeriod = atoi(schedule->FirstChildElement("cycle")->GetText());
    INFO("hyper period:\t" + std::to_string(hyperPeriod));
    /* get flows */
    XMLElement* device = schedule->FirstChildElement("host");
    while (strcmp(device->Attribute("name"), ConfigSetting::getInstance().get<const char*>("deviceName")) != 0) {
        device = device->NextSiblingElement();
    }
    XMLElement* entry = device->FirstChildElement("entry");
    while (entry) {
        Flow* flow = new Flow();
        flow->startOffset = Time::converIntegerToTimeInterval(
            atoi(entry->FirstChildElement("start")->GetText()),
            timeUnit
        );
        flow->durationInterval = Time::converIntegerToTimeInterval(
            atoi(entry->FirstChildElement("duration")->GetText()),
            timeUnit
        );
        flow->periodInterval = Time::converIntegerToTimeInterval(
            atoi(entry->FirstChildElement("period")->GetText()),
            timeUnit
        );
        flow->pcp = atoi(entry->FirstChildElement("pcp")->GetText());
        flow->size = ConvertUtils::converSizeToByte(
            atoi(entry->FirstChildElement("size")->GetText()), 
            sizeUnit
        );
        flow->destMac = entry->FirstChildElement("dest")->GetText();
        flow->groupMac = entry->FirstChildElement("group")->GetText();
        flow->uniqueID = atoi(entry->FirstChildElement("uniqueID")->GetText());
        INFO("[" + this->m_deviceName + "] add flow: " + flow->toString());
        this->flows.push_back(std::move(flow));
        this->flowNum += 1;
        entry = entry->NextSiblingElement();
    }
    std::sort(this->flows.begin(), this->flows.end(), _compare);
}

/* change current state */
void FlowApp::setState(const std::shared_ptr<IPortState>& state) {
    this->m_state = state;
}

/* return current state */
std::shared_ptr<IPortState>& FlowApp::getState() {
    return this->m_state;
}

/* create raw socket */
void FlowApp::createSocket() {
    /* create inbound socket */
    this->m_iSockfd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (this->m_iSockfd == -1) {
        ERROR(this->toString() + "] fail to create socket");
        throw PortCreationException();
    }
    INFO(this->toString() + " create inbound socket [" + std::to_string(this->m_iSockfd) + "]");
    /* create outbound socket */
    this->m_oSockfd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (this->m_oSockfd == -1) {
        ERROR(this->toString() + "] fail to create socket");
        throw PortCreationException();
    }
    INFO(this->toString() + " create outbound socket [" + std::to_string(this->m_oSockfd) + "]");
    /* create link layer interface */
    try {
        this->m_inf.reset(LinkLayerInterface::findInterface(this->m_deviceName.c_str()));
    } catch (const std::exception& e) {
        ERROR(e.what());
        throw PortCreationException();
    }
    INFO(this->toString() + " create interface: " + this->m_inf->toString());
    /* set MacAddress */
    std::shared_ptr<MacAddress> macAddress = this->m_inf->getMacAddress();
    macAddress->setFamily(PF_PACKET);    // set family
    macAddress->setProtocol(ETH_P_ALL);  // set protocol
    INFO(this->toString() + " create mac address: " + macAddress->toString());
    /* bind inbound socket to link layer interface */
    try {
        this->m_inf->bind(this->m_iSockfd, macAddress);
        INFO(this->toString() + " bind port to interface");
    } catch (const std::exception& e) {
        ERROR(e.what());
        throw PortCreationException();
    }
}

/* register handler */
void FlowApp::registerEventHandler() {
    /* fill in struct sockaddr_ll */
    struct sockaddr_ll* addr_ll;
    this->m_inf->getMacAddress()->getRawSockAddr((struct sockaddr**)&addr_ll);
    /* register for outbound socket */
    std::shared_ptr<IEventHandler> oHandler = std::make_shared<SendTSNFrameEventHandler>(this->m_oSockfd, *addr_ll, this);
    Reactor::getInstance().register_handler(oHandler, EVENT_TYPE::WRITE);
    INFO(this->toString() +  "register SendTSNFrameEventHandler in Reactor");
}

/* input something into port */
void FlowApp::input(void*, size_t, RELAY_ENTITY type) {
    // TODO
}

/* output something */
void* FlowApp::output() {
    INFO(this->toString() + " output oncall");
    if (this->frameCount == 0) {
        return nullptr;
    }

    /* construct tsn frame */
    Flow* flow = this->flows[this->flowIndex];
    INFO(this->toString() + " invoke flow " + flow->toString());
    TSNFrameBody* frame = new TSNFrameBody();
    int frameSize = MAX_FRAME_SIZE;
    if (this->frameCount == 1) {
        frameSize = this->lastFrameSize;
    }
    unsigned char* data = (unsigned char*)malloc(frameSize);
    memset(data, 0x00, frameSize);
    frame->setPCP(flow->pcp);
    frame->setVID(flow->vid);
    frame->setType(RELAY_ENTITY::IEEE_802_1Q_TSN_FRAME);
    frame->setSeq(flow->seq++);
    frame->setData(data, frameSize);
    INFO(this->toString() + " construct frame:" + frame->toString());

    this->frameCount -= 1;

    return (void*)frame;
}

void FlowApp::setTimer() {
    Flow* flow = this->flows[this->flowIndex];
    // this->lastFrameSize = flow->size % MAX_FRAME_SIZE;
    // this->frameCount = flow->size / MAX_FRAME_SIZE;
    // if (this->lastFrameSize != 0) {
    //     this->frameCount += 1;
    //     this->lastFrameSize = MAX_FRAME_SIZE;
    // }
    // INFO(this->toString() + "[f" + std::to_string(flow->uniqueID) + "] no. of frame = " + std::to_string(this->frameCount));

    // // TODO add ticker
    // INFO(this->toString() + "[f" + std::to_string(flow->uniqueID) + "] register to timer");
    // Time::TimePoint startTime(0, 0);
    // Ticker* ticker = new FlowTicker(startTime, flow->periodInterval, this);
    // TimeContext::getInstance().getTimer()->addTicker(ticker);

    registerFlowToTimer(this, flow, flow->periodInterval);
}

void registerFlowToTimer(FlowApp* app, Flow* flow, Time::TimeInterval& interval) {
    app->lastFrameSize = flow->size % MAX_FRAME_SIZE;
    app->frameCount = flow->size / MAX_FRAME_SIZE;
    if (app->lastFrameSize != 0) {
        app->frameCount += 1;
        app->lastFrameSize = MAX_FRAME_SIZE;
    }
    INFO(app->toString() + "[f" + std::to_string(flow->uniqueID) + "] no. of frame = " + std::to_string(app->frameCount));

    // TODO add ticker
    INFO(app->toString() + "[f" + std::to_string(flow->uniqueID) + "] register to timer");
    Time::TimePoint startTime(0, 0);
    Ticker* ticker = new FlowTicker(startTime, interval, app);
    TimeContext::getInstance().getTimer()->addTicker(ticker);
}
    
} // namespace faker_tsn
