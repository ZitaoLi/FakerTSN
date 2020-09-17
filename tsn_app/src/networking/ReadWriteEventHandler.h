#ifndef _READ_WRITE_EVENT_HANDLER
#define _READ_WRITE_EVENT_HANDLER

#include "IEventHandler.h"

namespace faker_tsn
{

class ReadWriteEventHandler: public IEventHandler
{
private:
    HANDLE m_handle;
public:
    ReadWriteEventHandler(HANDLE handle);
    virtual ~ReadWriteEventHandler() override;

    virtual void handle_event(EVENT_TYPE eventType) override;

    virtual HANDLE getHandle() override;
};
    
} // namespace faker_tsn


#endif // _READ_WRITE_EVENT_HANDLER