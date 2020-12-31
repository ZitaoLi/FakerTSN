#ifndef _I_FRAME_BODY_H
#define _I_FRAME_BODY_H

#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <string>
#include <string.h>

#include "RelayEntity.h"

namespace faker_tsn {

class IFrameBody {
   protected:
    RELAY_ENTITY m_type;
    unsigned char m_data[ETH_DATA_LEN];
    unsigned int m_bytes;

   public:
    virtual ~IFrameBody() = default;

    void setData(unsigned char* data, unsigned int len) {
        memcpy(this->m_data, data, len);
        this->m_bytes = len;
    }

    void* getData(unsigned char* buf, unsigned int len) {
        memcpy(buf, this->m_data, len);
    }

    unsigned int getBytes() {
        return this->m_bytes;
    }

    virtual std::string toString() {
        return "IFrameBody";
    }

    void setType(RELAY_ENTITY type) {
        this->m_type = type;
    }

    RELAY_ENTITY getType() {
        return this->m_type;
    }
};

}  // namespace faker_tsn

#endif  // _I_FRAME_BODY_H