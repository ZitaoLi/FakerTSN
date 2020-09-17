#include "ReadEventHandler.h"

namespace faker_tsn {

ReadEventHandler::ReadEventHandler(HANDLE handle) : m_handle(handle) {
}

ReadEventHandler::~ReadEventHandler() {
}

HANDLE ReadEventHandler::getHandle() {
    return this->m_handle;
}

void ReadEventHandler::handle_event(EVENT_TYPE eventType) {
    assert(eventType == EVENT_TYPE::READ);
    char *buff = (char *)malloc(sizeof(char) * 100);
    size_t nbytes = 100;
    int fd = this->m_handle;
    if (read(fd, (void *)buff, nbytes) == -1) {
        ERROR("failed to read");
    } else {
        INFO(buff);
    }
    free(buff);
}

}  // namespace faker_tsn
