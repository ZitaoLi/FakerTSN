#ifndef _SEND_TSN_FRAME_EVENT_HANDLER_H
#define _SEND_TSN_FRAME_EVENT_HANDLER_H

#include <linux/if_packet.h>

#include <memory>

#include "../core/queue/QueueContext.h"
#include "../core/forward/MacTable.h"
#include "../core/forward/MulticastMacTable.h"
#include "../utils/Log.h"
#include "../utils/container/ConvertUtils.h"
#include "IEventHandler.h"
#include "Reactor.h"
#include "VlanTCI.h"
#include "RTCI.h"
#include "frame/TSNFrameBody.h"
#include "if_ether.h"

namespace faker_tsn {

class SendTSNFrameEventHandler : public IEventHandler {
   private:
    HANDLE m_handle;
    struct sockaddr_ll m_sockAddrII;
    IPort* m_port;
    bool m_isEnhanced;

   public:
    SendTSNFrameEventHandler(HANDLE handle, struct sockaddr_ll& sockAddrII, IPort* port);

    virtual ~SendTSNFrameEventHandler() override;

    /* hook method that is called by Reactor */
    virtual void handle_event(EVENT_TYPE eventType) override;

    /* hook method that returns smart pointer of HANDLE */
    virtual HANDLE getHandle() override;
};

}  // namespace faker_tsn

#endif  // _SEND_TSN_FRAME_EVENT_HANDLER_H