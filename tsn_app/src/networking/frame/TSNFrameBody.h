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
    unsigned short m_pcp;               /* priority code point */
    unsigned short m_vid;               /* vlan id */
    unsigned short m_seq;               /* sequencr nubmer */
    
   public:
    TSNFrameBody() = default;

    virtual ~TSNFrameBody() { INFO("Destruct TSNFrameBody"); };

    std::string toString() {
        std::stringstream ss;
        ss << "pcp[" << this->m_pcp << "] ";
        ss << "vid[" << this->m_vid << "] ";
        ss << "seq[" << this->m_seq << "] ";
        ss << "bytes[" << this->m_bytes << "] ";
        return ss.str();
    }

    void setPCP(unsigned short pcp) {
        this->m_pcp = pcp;
    }

    void setVID(unsigned short vid) {
        this->m_vid = vid;
    }

    void setSeq(unsigned short seq) {
        this->m_seq = seq;
    }

    unsigned short getPCP() {
        return this->m_pcp;
    }

    unsigned short getVID() {
        return this->m_vid;
    }

    unsigned short getSeq() {
        return this->m_seq;
    }
};

}  // namespace faker_tsn

#endif