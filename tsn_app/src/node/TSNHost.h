#ifndef _TSN_HOST_H
#define _TSN_HOST_H

#include "INode.h"

#include <vector>
#include <memory>
#include <string>

#include "../utils/config/ConfigSetting.h"
#include "../utils/container/ConvertUtils.h"
#include "../utils/Log.h"
#include "../timer/ITimer.h"
#include "../core/port/IPort.h"
#include "../core/TSNContext.h"
#include "../core/InitTSNContextState.h"
#include "../networking/interface/LinkLayerInterface.h"
#include "../networking/SendTSNFrameEventHandler.h"
#include "FlowApp.h"

namespace faker_tsn {

class TSNHost: public INode {
private:
    std::string m_deviceName;

public:
    TSNHost();

    virtual ~TSNHost() override;

    virtual void run() override;

    virtual std::string toString() {
        std::stringstream ss;
        ss << "[" << this->m_deviceName << "]";
        return ss.str();
    }
};
    
} // namespace faker_tsn

#endif // _TSN_HOST_H