#include "ReadWriteEventHandler.h"

namespace faker_tsn {

ReadWriteEventHandler::ReadWriteEventHandler(HANDLE handle) : m_handle(handle) {
}

ReadWriteEventHandler::~ReadWriteEventHandler() {
}

HANDLE ReadWriteEventHandler::getHandle() {
    return this->m_handle;
}

void ReadWriteEventHandler::handle_event(EVENT_TYPE eventType) {
    assert(eventType == EVENT_TYPE::WRITE || eventType == EVENT_TYPE::READ);

    if (eventType == EVENT_TYPE::READ) {
        // TODO
    } else {
        std::string testStr("test write");
        int fd = this->m_handle;
        if (write(fd, testStr.c_str(), 5)) {
            ERROR("failed to write");
        } else {
            INFO("write successful");
        }
    }
}

}  // namespace faker_tsn
