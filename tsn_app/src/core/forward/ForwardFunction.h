#ifndef _FORWARD_FUNCTION_H
#define _FORWARD_FUNCTION_H

#include <vector>

#include "../../networking/frame/RelayEntity.h"

namespace faker_tsn {

class ForwardFunction {
   public:
    static void forward(const unsigned char* srcMac, const unsigned char* destMac, void* data, size_t len, RELAY_ENTITY type);

   public:
    static const char s_multicastPrefix;
};

}  // namespace faker_tsn

#endif  // _FORWARD_FUNCTION_H