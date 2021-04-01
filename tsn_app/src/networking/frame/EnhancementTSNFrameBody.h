#ifndef _ENHANCEMENT_TSN_FRAME_BODY_H
#define _ENHANCEMENT_TSN_FRAME_BODY_H

#include "TSNFrameBody.h"

namespace faker_tsn {

class EnhancementTSNFrameBody : public IFrameBody {
   protected:
    RELAY_ENTITY m_type;
    unsigned char m_data[ETH_DATA_LEN];
    unsigned int m_bytes;
    uint8_t m_pcp;                  /* priority code point */
    uint16_t m_vid;                 /* vlan id */
    uint16_t m_seq;                 /* sequencr nubmer */
    unsigned long m_flowId;         /* flow id */
    unsigned int m_phase;           /* phase */
    unsigned char m_mac[ETH_ALEN];  /* group mac */

   public:
    EnhancementTSNFrameBody() {}

    virtual ~EnhancementTSNFrameBody() {}

    virtual std::string toString() {
        std::stringstream ss;
        ss << "pcp[" << std::to_string(this->m_pcp) << "] ";
        ss << "vid[" << std::to_string(this->m_vid) << "] ";
        ss << "seq[" << std::to_string(this->m_seq ) << "] ";
        ss << "bytes[" << std::to_string(this->m_bytes) << "] ";
        ss << "fid[" << std::to_string(this->m_flowId) << "] ";
        ss << "phs[" << std::to_string(this->m_phase) << "] ";
        return ss.str();
    }

    virtual IFrameBody* copy() override {
        EnhancementTSNFrameBody* oldFrame = this;
        EnhancementTSNFrameBody* newFrame = new EnhancementTSNFrameBody();
        newFrame->setType(oldFrame->getType());
        unsigned int len = oldFrame->getBytes();
        unsigned char* buf = (unsigned char*)malloc(len);
        oldFrame->getData(buf, len);
        newFrame->setData(buf, len);
        newFrame->setPCP(oldFrame->getPCP());
        newFrame->setSeq(oldFrame->getSeq());
        newFrame->setVID(oldFrame->getVID());
        newFrame->setFlowId(oldFrame->getFlowId());
        newFrame->setPhse(oldFrame->getPhase());
        unsigned char mac[ETH_ALEN];
        oldFrame->getMac(mac);
        newFrame->setMac(mac);
        free(buf);
        return newFrame;
    }

    virtual void setData(unsigned char* data, unsigned int len) override {
        memcpy(this->m_data, data, len);
        this->m_bytes = len;
    }

    virtual void* getData(unsigned char* buf, unsigned int len) {
        memcpy(buf, this->m_data, len);
    }

    virtual unsigned int getBytes() {
        return this->m_bytes;
    }

    virtual void setType(RELAY_ENTITY type) {
        this->m_type = type;
    }

    virtual RELAY_ENTITY getType() {
        return this->m_type;
    }

   public:
    void setPCP(uint8_t pcp) {
        this->m_pcp = pcp;
    }

    void setVID(uint16_t vid) {
        this->m_vid = vid;
    }

    void setSeq(uint16_t seq) {
        this->m_seq = seq;
    }

    uint8_t getPCP() {
        return this->m_pcp;
    }

    uint16_t getVID() {
        return this->m_vid;
    }

    uint16_t getSeq() {
        return this->m_seq;
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