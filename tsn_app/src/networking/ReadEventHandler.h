#ifndef _READ_EVENT_HANDLER
#define _READ_EVENT_HANDLER

#include "IEventHandler.h"

namespace faker_tsn
{

class ReadEventHandler: public IEventHandler
{
private:
    HANDLE m_handle;
public:
    ReadEventHandler(HANDLE handle);
    virtual ~ReadEventHandler() override;

    virtual void handle_event(EVENT_TYPE eventType) override;

    virtual HANDLE getHandle() override;
};
    
} // namespace faker_tsn


#endif // _READ_EVENT_HANDLER