#ifndef _CONTROL_EVENT_HANDLER_H
#define _CONTROL_EVENT_HANDLER_H

#include <vector>
#include <string>
#include <iostream>
#include <ctime>

#include "IEventHandler.h"

namespace faker_tsn
{

class ControlEventHandler: public IEventHandler {
private:
    HANDLE m_handle;

public:
    ControlEventHandler(HANDLE handle);
    
    virtual ~ControlEventHandler() override;

    virtual void handle_event(EVENT_TYPE eventType) override;

    virtual HANDLE getHandle() override;
};
    
} // namespace faker_tsn


#endif // _CONTROL_EVENT_HANDLER_H