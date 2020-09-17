#ifndef _COMMUNICATOR_H
#define _COMMUNICATOR_H

namespace faker_tsn
{

#include <netinet/if_ether.h>
#include "if_ether.h"

class communicator
{
private:
    /* data */
public:
    virtual ~communicator() = default;
};
    
} // namespace faker_tsn


#endif
