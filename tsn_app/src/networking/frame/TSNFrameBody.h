#ifndef _TSN_FRAME_BODY_H
#define _TSN_FRAME_BODY_H

#include <memory.h>
#include <string>
#include <sstream>

#include "../../utils/Log.h"
#include "../if_ether.h"
#include "IFrameBody.h"

namespace faker_tsn {

class TSNFrameBody : public IFrameBody {
   protected:
    RELAY_ENTITY m_type;
    unsigned char m_data[ETH_DATA_LEN];
    unsigned int m_bytes;
    uint8_t m_pcp;               /* priority code point */
    uint16_t m_vid;               /* vlan id */
    uint16_t m_seq;               /* sequencr nubmer */
    
   public:
    TSNFrameBody() {}

    virtual ~TSNFrameBody() { INFO("Destruct TSNFrameBody"); };

    virtual std::string toString() {
        std::stringstream ss;
        ss << "pcp[" << std::to_string(this->m_pcp) << "] ";
        ss << "vid[" << std::to_string(this->m_vid) << "] ";
        ss << "seq[" << std::to_string(this->m_seq ) << "] ";
        ss << "bytes[" << std::to_string(this->m_bytes) << "] ";
        return ss.str();
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
};

}  // namespace faker_tsn

#endif