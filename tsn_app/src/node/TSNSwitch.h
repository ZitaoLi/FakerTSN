#ifndef _TSN_SWITCH_H
#define _TSN_SWITCH_H

#include "INode.h"

#include "../core/TSNContext.h"
#include "../core/InitTSNContextState.h"

namespace faker_tsn
{

class TSNSwitch: public INode
{
public:
    TSNSwitch();

    virtual ~TSNSwitch() override;

    virtual void run() override;
};

    
} // namespace faker_tsn


#endif // _TSN_SWITCH_H