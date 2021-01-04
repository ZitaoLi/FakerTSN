#include "TSNController.h"

namespace faker_tsn
{

TSNController::TSNController(const char* deviceName) : 
    m_deviceName(deviceName), 
    m_command("") {
    INFO(this->toString() + " construct FlowApp");
}

TSNController::~TSNController() {
    INFO(this->toString() + " desstruct FlowApp");
    shutdown(this->m_iSockfd, SHUT_RDWR);
    shutdown(this->m_oSockfd, SHUT_RDWR);
    close(this->m_iSockfd);
    close(this->m_oSockfd);
}

/* change current state */
void TSNController::setState(const std::shared_ptr<IPortState>& state) {
    this->m_state = state;
}

/* return current state */
std::shared_ptr<IPortState>& TSNController::getState() {
    return this->m_state;
}

void TSNController::createSocket() {
    /* create cammand socket */
    this->m_csockfd = STDIN_FILENO;
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

void TSNController::registerEventHandler() {
    /* fill in struct sockaddr_ll */
    struct sockaddr_ll* addr_ll;
    this->m_inf->getMacAddress()->getRawSockAddr((struct sockaddr**)&addr_ll);
    /* register for outbound socket */
    std::shared_ptr<IEventHandler> oHandler = std::make_shared<CommandEventHandler>(this->m_oSockfd, *addr_ll, this);
    Reactor::getInstance().register_handler(oHandler, EVENT_TYPE::WRITE);
    INFO(this->toString() +  "register CommandEventHandler in Reactor");
}

void TSNController::input(void* data, size_t len, RELAY_ENTITY type) {
    char buf[COMMAND_SIZE];
    memset(buf, 0x00, COMMAND_SIZE);
    memcpy(buf, data, len);
    this->m_command = std::string(buf);
    INFO(this->toString() + " <<< " + this->m_command);
    // enable EPOLLOUT event
    Reactor::getInstance().getDemultoplexer().updateHandle(this->m_oSockfd, EPOLLOUT);
}

void* TSNController::output() {
    // disable EPOLLOUT event
    Reactor::getInstance().getDemultoplexer().updateHandle(this->m_oSockfd, 0x00);
    if (this->m_command.empty())
        return nullptr;
    return &this->m_command;
}
    
} // namespace faker_tsn
