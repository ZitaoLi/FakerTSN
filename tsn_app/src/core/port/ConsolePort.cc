#include "ConsolePort.h"

#include <fcntl.h>

#include "CreationPortState.h"

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
}

void ConsolePort::registerEventHandler() {
    std::shared_ptr<IEventHandler> stdinEventHandler = std::make_shared<StdinEventHandler>(this->m_sockfd);
    /* register stdin event handler */
    Reactor::getInstance().register_handler(stdinEventHandler, EVENT_TYPE::READ);
}

void ConsolePort::sendTest() {
}

/* input something into port */
void ConsolePort::input(void* data, size_t len, RELAY_ENTITY type) {}

}  // namespace faker_tsn
