#ifndef _STDIN_EVENT_HANDLER
#define _STDIN_EVENT_HANDLER

#include "IEventHandler.h"

namespace faker_tsn
{

class StdinEventHandler: public IEventHandler
{
private:
    HANDLE m_handle;
public:
    StdinEventHandler(HANDLE handle);
    virtual ~StdinEventHandler() override;

    virtual void handle_event(EVENT_TYPE eventType) override;

    virtual HANDLE getHandle() override;
};
    
} // namespace faker_tsn


#endif // _STDIN_EVENT_HANDLER