#ifndef _COMMAND_EVENT_HANDLER_H
#define _COMMAND_EVENT_HANDLER_H

#include <vector>
#include <string>

#include "../networking/IEventHandler.h"
#include "../timer/ITimer.h"
#include "../timer/TimeContext.h"
#include "../core/port/IPort.h"
#include "../utils/reflector/Reflector.h"

namespace faker_tsn
{

class CommandEventHandler : public IEventHandler {
private:
    HANDLE m_handle;
    struct sockaddr_ll m_sockAddrII;
    IPort* m_port;

private:
    void handle_input();

    void handle_output();

public:
    CommandEventHandler(HANDLE handle, struct sockaddr_ll& sockAddrII, IPort* port);

    virtual ~CommandEventHandler() override;

    /* hook method that is called by Reactor */ 
    virtual void handle_event(EVENT_TYPE eventType) override;

    /* hook method that returns smart pointer of HANDLE */
    virtual HANDLE getHandle() override {
        return this->m_handle;
    }
};
    
} // namespace faker_tsn


#endif // _COMMAND_EVENT_HANDLER_H