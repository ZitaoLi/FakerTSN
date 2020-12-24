#include "DataPort.h"
#include "../../utils/config/ConfigSetting.h"

#include <sys/socket.h>

namespace faker_tsn {

uint8_t DataPort::s_portNum = 0;

DataPort::DataPort(const char* deviceName) : m_deviceID(DataPort::s_portNum++), m_deviceName(deviceName) {
    INFO("Construct Port[" + std::to_string(s_portNum) + "|" + std::string(this->m_deviceName) + "] created");
}

DataPort::~DataPort() {
    /* close socket */
    shutdown(this->m_sockfd, SHUT_RDWR);
    close(this->m_sockfd);

    INFO("Destruct " + std::to_string(s_portNum) + "|" + std::string(this->m_deviceName) + "] closed");
}

void DataPort::setState(const std::shared_ptr<IPortState>&) {
    this->state = state;
}

std::shared_ptr<IPortState>& DataPort::getState() {
    return this->state;
}

uint8_t DataPort::getDeviceID() const {
    // return this->m_deviceID;
    return 0;
}

void DataPort::setSockfd(int sockfd) {
    this->m_sockfd = sockfd;
}

int DataPort::getSockfd() {
    return this->m_sockfd;
}

void DataPort::setMacAddress(const char mac[]) {
    memcpy(this->m_hwaddr, mac, sizeof(this->m_hwaddr));
}

void DataPort::setDeviceName(const char* deviceName) {
    this->m_deviceName = deviceName;
}

const char* DataPort::getDeviceName() const {
    return this->m_deviceName;
}

void DataPort::setEventTypes(int events) {
    this->m_eventTypes = events;
}

int DataPort::getEventTypes() {
    return this->m_eventTypes;
}

void DataPort::createSocket() {
    // int sockfd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_TSN));
    // if (sockfd == -1) {
    //     ERROR("Port[" + std::string(this->getDeviceName()) + "] fail to create socket");
    //     throw PortCreationException();
    // }
    // this->setSockfd(sockfd);
    // INFO("Port[" + std::string(this->getDeviceName()) + "] set sockfd: " + std::to_string(sockfd));

    /* create inbound socket */
    int inSockfd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (inSockfd == -1) {
        ERROR(this->toString() + "] fail to create socket");
        throw PortCreationException();
    }
    this->setInSockfd(inSockfd);
    INFO("Create inbound socket: " + std::to_string(this->getInSockfd()));

    /* create outbound socket */
    int outSockfd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    if (outSockfd == -1) {
        ERROR(this->toString() + "] fail to create socket");
        throw PortCreationException();
    }
    this->setOutSockfd(outSockfd);
    INFO("Create outbound socket: " + std::to_string(this->getOutSockfd()));

    /* create LinkLayerInterface */
    try {
        this->m_interface.reset(LinkLayerInterface::findInterface(this->m_deviceName));
    } catch (const std::exception& e) {
        ERROR(e.what());
        throw PortCreationException();
    }
    INFO("Create LinkLayerInterface:\n" + this->m_interface->toString());

    /* set MacAddress */
    std::shared_ptr<MacAddress> macAddress = this->m_interface->getMacAddress();
    macAddress->setFamily(PF_PACKET);    // set family
    macAddress->setProtocol(ETH_P_ALL);  // set protocol
    INFO("Create MacAddress");

    /* bind inbound socket to link layer interface */
    try {
        this->m_interface->bind(this->m_inSockfd, macAddress);
        INFO("Bind port with interface");
    } catch (const std::exception& e) {
        ERROR(e.what());
        throw PortCreationException();
    }

    /* create QueueContext */
    INFO("Create QueueConext");
    this->m_queueContext.reset(new QueueContext(this->m_deviceID));

    /* create GateControlList */
    ConfigSetting& cs = ConfigSetting::getInstance();
    std::string enabledTSN = cs.get<std::string>("enabledTSN");
    if (enabledTSN == "yes") {
        std::string gclClass = "faker_tsn::" + cs.get<std::string>("switch.gcl");
        INFO("[" + std::string(this->m_deviceName) + "][" + std::to_string(this->m_deviceID) + "] Create " + gclClass);
        std::shared_ptr<GateControlList> gcl(dynamic_cast<GateControlList*>(REFLECTOR::CreateByTypeName(gclClass, (unsigned int)this->m_deviceID)));
        this->m_gcl = gcl;
        // GateControlList* gcl = dynamic_cast<GateControlList*>(REFLECTOR::CreateByTypeName(gclClass, (unsigned int)this->m_deviceID));
        // this->m_gcl.reset(gcl);
        std::vector<std::shared_ptr<IQueue>> queues = this->m_queueContext->getQueues();
        for (std::shared_ptr<IQueue> queue: queues) {
            auto gate = std::dynamic_pointer_cast<IEEE8021QbvQueue>(queue)->getTransmissionGate();
            gate->registerGCL(this->m_gcl);
        }
        this->m_gcl->updateGates(); // first update
    }

    /* mac address */
    // char hwaddr[ETH_ALEN];
    // if (PortManager::findMacAddress(shared_from_this(), this->getDeviceName(), hwaddr)) {
    //     ERROR("Port[" + std::string(this->getDeviceName()) + "] fail to get mac address");
    //     throw PortCreationException();
    // }
    // this->setMacAddress(hwaddr);
    // /* hex array to string */
    // std::stringstream ss;
    // ss.fill('0');
    // for (size_t i = 0; i < ETH_ALEN; ++i)
    //     ss << std::setw(2) << std::hex << (unsigned short)hwaddr[i] << " ";
    // INFO("Port[" + std::string(this->getDeviceName()) + "] set mac address: " + ss.str());
}

void DataPort::registerEventHandler() {
    /* fill in struct sockaddr_ll */
    struct sockaddr_ll* addr_ll;
    this->m_interface->getMacAddress()->getRawSockAddr((struct sockaddr**)&addr_ll);

    /* register for inbound socket */
    std::shared_ptr<IEventHandler> recvTSNFrameEventHandler = std::make_shared<RecvTSNFrameEventHandler>(this->m_inSockfd, *addr_ll);
    // std::shared_ptr<IEventHandler> recvTSNFrameEventHandler(dynamic_cast<IEventHandler*>(REFLECTOR::CreateByTypeName(
    //     "faker_tsn::RecvTSNFrameEventHandler", 
    //     (int)this->m_inSockfd, 
    //     (struct sockaddr_ll)(*addr_ll)))
    // );
    Reactor::getInstance().register_handler(recvTSNFrameEventHandler, EVENT_TYPE::READ);
    INFO("Register RecvTSNFrameEventHandler in Reactor");

    /* register for outbound socket */
    std::shared_ptr<IEventHandler> sendTSNFrameEventHandler = std::make_shared<SendTSNFrameEventHandler>(this->m_outSockfd, *addr_ll, this);
    Reactor::getInstance().register_handler(sendTSNFrameEventHandler, EVENT_TYPE::WRITE);
    INFO("Register SendTSNFrameEventHandler in Reactor");
}

void DataPort::sendTest() {
    INFO("TEST SEND");
    union ethframe {
        struct
        {
            struct ethhdr header;
            unsigned char data[ETH_DATA_LEN];
        } field;
        unsigned char buffer[ETH_FRAME_LEN];
    };
    /* get interface index */
    struct ifreq buffer;
    int ifindex;
    memset(&buffer, 0x00, sizeof(buffer));
    strncpy(buffer.ifr_name, this->m_deviceName, IFNAMSIZ);
    if (ioctl(this->m_sockfd, SIOCGIFINDEX, &buffer) < 0) {
        printf("Error: could not get interface index\n");
        // TODO handle error
    }
    ifindex = buffer.ifr_ifindex;

    /* fill in frame */
    union ethframe frame;
    unsigned char dest[ETH_ALEN] = {0x00, 0x12, 0x34, 0x56, 0x78, 0x90};
    unsigned short proto = ETH_P_TSN;
    const char* data = "hello world\n";
    unsigned short data_len = strlen(data);
    memcpy(frame.field.header.h_dest, dest, ETH_ALEN);
    memcpy(frame.field.header.h_source, this->m_hwaddr, ETH_ALEN);
    frame.field.header.h_proto = htons(proto);
    memcpy(frame.field.data, data, data_len);

    unsigned int frame_len = data_len + ETH_HLEN;

    /* set linklayer address */ 
    struct sockaddr_ll saddrll;
    memset((void*)&saddrll, 0, sizeof(saddrll));
    saddrll.sll_family = PF_PACKET;
    saddrll.sll_ifindex = ifindex;
    saddrll.sll_halen = ETH_ALEN;
    memcpy((void*)(saddrll.sll_addr), (void*)dest, ETH_ALEN);

    if (write(STDIN_FILENO, frame.buffer, frame_len)) {
        INFO("Write success!");
    } else {
        INFO("Write error!");
    }

    /* send data */
    if (sendto(this->m_sockfd, frame.buffer, frame_len, 0, (struct sockaddr*)&saddrll, sizeof(saddrll)) > 0)
        INFO("Send success!");
    else
        INFO("Error, could not send");
}

void DataPort::input(void* data, size_t len, RELAY_ENTITY type) {
    if (type == IEEE_802_1Q_TSN_FRAME) {
        INFO("Input TSN frame");
        TSNFrameBody* frame = reinterpret_cast<TSNFrameBody*>(data);
        frame->setType(type);
        this->m_queueContext->enqueue(frame);
    } else if (type == IEEE_802_1Q_TSN_FRAME_E) {
        INFO("Input enhanced-TSN frame");
        EnhancementTSNFrameBody* frame = reinterpret_cast<EnhancementTSNFrameBody*>(data);
        frame->setType(type);
        this->m_queueContext->enqueue(reinterpret_cast<EnhancementTSNFrameBody*>(data));
    }
}

void* DataPort::output() {
    return (void*)this->m_queueContext->dequeue();
}

void DataPort::setTimer() {
    /* register gate control list item into timer */
    INFO("register GCL items");
    this->m_gcl->registerGCLfromSchedules();
}

std::shared_ptr<QueueContext> DataPort::getQueueContext() {
    return this->m_queueContext;
}

}  // namespace faker_tsn
