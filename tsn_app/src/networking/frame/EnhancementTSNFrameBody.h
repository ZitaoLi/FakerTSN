#ifndef _ENHANCEMENT_TSN_FRAME_BODY_H
#define _ENHANCEMENT_TSN_FRAME_BODY_H

#include "TSNFrameBody.h"

namespace faker_tsn {

class EnhancementTSNFrameBody : public TSNFrameBody {
   protected:
    unsigned long m_flowId;         /* flow id */
    unsigned int m_phase;           /* phase */
    unsigned char m_mac[ETH_ALEN];  /* group mac */

   public:
    EnhancementTSNFrameBody() {}

    virtual ~EnhancementTSNFrameBody() {}

    std::string toString() {
        std::stringstream ss;
        ss << "pcp[" << this->m_pcp << "] ";
        ss << "vid[" << this->m_vid << "] ";
        ss << "seq[" << this->m_seq << "] ";
        ss << "bytes[" << this->m_bytes << "] ";
        ss << "fid[" << this->m_flowId << "] ";
        ss << "phs[" << this->m_phase << "] ";
        return ss.str();
    }

    inline void setFlowId(unsigned long flowId) {
        this->m_flowId = flowId;
    }

    inline void setPhse(unsigned int phase) {
        this->m_phase = phase;
    }

    inline void setMac(const unsigned char* mac) {
        memcpy(this->m_mac, mac, ETH_ALEN);
    }

    inline unsigned long getFlowId() const {
        return this->m_flowId;
    }

    inline unsigned int getPhase() const {
        return this->m_phase;
    }

    inline void* getMac(unsigned char* mac) const {
        return memcpy(mac, this->m_mac, ETH_ALEN);
    }
};

}  // namespace faker_tsn

#endif  // _ENHANCEMENT_TSN_FRAME_BODY_H