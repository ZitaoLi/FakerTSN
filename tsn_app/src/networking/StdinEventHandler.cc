#include "StdinEventHandler.h"

namespace faker_tsn
{

StdinEventHandler::StdinEventHandler(HANDLE handle): m_handle(handle)
{

}

StdinEventHandler::~StdinEventHandler()
{

}

HANDLE StdinEventHandler::getHandle()
{
    return this->m_handle;
}

void StdinEventHandler::handle_event(EVENT_TYPE eventType)
{
    assert(eventType == EVENT_TYPE::READ);
    char *buff = (char*)malloc(sizeof(char) * 100);
    size_t nbytes = 99;
    int fd = this->m_handle;
    FILE *file = fdopen(fd, "r");
    fgets(buff, nbytes, file);
    INFO(buff);

    free(buff);
}
    
} // namespace faker_tsn
