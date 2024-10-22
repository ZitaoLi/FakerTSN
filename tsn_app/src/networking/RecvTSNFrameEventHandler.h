#ifndef _RECV_TSN_FRAME_EVENT_HANDLER_H
#define _RECV_TSN_FRAME_EVENT_HANDLER_H

#include <linux/if_packet.h>
#include <random>

#include "../core/forward/ForwardFunction.h"
#include "../core/forward/MacTable.h"
#include "../core/queue/QueueContext.h"
#include "../networking/Reactor.h"
#include "../networking/frame/RelayEntity.h"
#include "../utils/Log.h"
#include "../utils/config/ConfigSetting.h"
#include "../utils/container/ConvertUtils.h"
#include "../utils/monitor/DataSpot.h"
#include "IEventHandler.h"
#include "VlanTCI.h"
#include "RTCI.h"
#include "frame/TSNFrameBody.h"
#include "if_ether.h"

namespace faker_tsn {

class RecvTSNFrameEventHandler : public IEventHandler {
   private:
    HANDLE m_handle;
    struct sockaddr_ll m_sockAddrII;
    IPort* m_port;
    std::shared_ptr<QueueContext> m_queueContext;
    bool m_isEnhanced;
    bool m_isSwitch;
    std::default_random_engine m_e;
    std::bernoulli_distribution m_u;

   public:
    RecvTSNFrameEventHandler(HANDLE handle, struct sockaddr_ll& sockAddrII, IPort* port);

    virtual ~RecvTSNFrameEventHandler() override;

    /* hook method that is called by Reactor */                                                                                   
    virtual void handle_event(EVENT_TYPE eventType) override;

    /* hook method that returns smart pointer of HANDLE */
    virtual HANDLE getHandle() override;

   public:
    static const __be16 s_protocol;
};

}  // namespace faker_tsn

#endif  // _RECV_TSN_FRAME_EVENT_HANDLER_H