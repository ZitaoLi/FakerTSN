#ifndef _I_FRAME_BODY_H
#define _I_FRAME_BODY_H

#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <string>
#include <string.h>

#include "RelayEntity.h"

namespace faker_tsn {

class IFrameBody {
   public:
    virtual ~IFrameBody() = default;

    virtual void setData(unsigned char* data, unsigned int len) = 0;

    virtual void* getData(unsigned char* buf, unsigned int len) = 0;

    virtual unsigned int getBytes() = 0;

    virtual std::string toString() = 0;

    virtual void setType(RELAY_ENTITY type) = 0;

    virtual RELAY_ENTITY getType() = 0;
};

}  // namespace faker_tsn

#endif  // _I_FRAME_BODY_H