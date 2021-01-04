#include "ConsolePort.h"

namespace faker_tsn {

ConsolePort::ConsolePort() {
    /* set NIO */
    fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
}

ConsolePort::~ConsolePort() {
}

void ConsolePort::setState(const std::shared_ptr<IPortState>&) {
    this->state = state;
}

std::shared_ptr<IPortState>& ConsolePort::getState() {
    return this->state;
}

uint8_t ConsolePort::getDeviceID() const {
    // return this->m_deviceID;
    return 0;
}

void ConsolePort::setSockfd(int sockfd) {
    this->m_sockfd = sockfd;
}

int ConsolePort::getSockfd() {
    return this->m_sockfd;
}

void ConsolePort::setMacAddress(const char mac[]) {
}

void ConsolePort::setDeviceName(const char* deviceName) {
}

void ConsolePort::setEventTypes(int events) {
    this->m_eventTypes = events;
}

int ConsolePort::getEventTypes() {
    return this->m_eventTypes;
}

const char* ConsolePort::getDeviceName() const {
    return "";
}

void ConsolePort::createSocket() {
    std::string deviceName = ConfigSetting::getInstance().get<std::string>("switch.port.cons");
    if (deviceName == "stdin") {
        this->m_sockfd = STDIN_FILENO; // standard input
        INFO(this->toString() + " console socket is STDIN");
    } else {
        this->m_sockfd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
        if (this->m_sockfd == -1) {
            ERROR(this->toString() + " fail to create socket");
            throw PortCreationException();
        }
        INFO(this->toString() + " console port is " + std::to_string(this->m_sockfd));
        /* create LinkLayerInterface */
        LinkLayerInterface* interface;
        try {
            interface = LinkLayerInterface::findInterface(deviceName.c_str());
        } catch (const std::exception& e) {
            ERROR(e.what());
            throw PortCreationException();
        }
        this->m_interface.reset(interface);
        INFO(this->toString() + " create LinkLayerInterface:\n" + interface->toString());
        /* set MacAddress */
        std::shared_ptr<MacAddress> macAddress = interface->getMacAddress();
        macAddress->setFamily(PF_PACKET);    // set family
        macAddress->setProtocol(ETH_P_ALL);  // set protocol
        INFO("Create MacAddress");
        /* bind inbound socket to link layer interface */
        try {
            interface->bind(this->m_sockfd, macAddress);
            INFO(this->toString() + " bind port with interface");
        } catch (const std::exception& e) {
            ERROR(e.what());
            throw PortCreationException();
        }
    }
}

void ConsolePort::registerEventHandler() {
    /* fill in struct sockaddr_ll */
    struct sockaddr_ll* addr_ll;
    if (this->m_sockfd != STDIN_FILENO)
        this->m_interface->getMacAddress()->getRawSockAddr((struct sockaddr**)&addr_ll);
    /* register handler */
    std::shared_ptr<IEventHandler> handler;
    std::string workMode = ConfigSetting::getInstance().get<std::string>("workMode");
    if (workMode == "configurator") {
        handler = std::make_shared<CommandEventHandler>(this->m_sockfd, *addr_ll, this);
    } else {
        handler = std::make_shared<ControlEventHandler>(this->m_sockfd);
    }
    Reactor::getInstance().register_handler(handler, EVENT_TYPE::READ);
}

void ConsolePort::sendTest() {
}

/* input something into port */
void ConsolePort::input(void* data, size_t len, RELAY_ENTITY type) {}

void* ConsolePort::output() {
    return nullptr;
}

}  // namespace faker_tsn
