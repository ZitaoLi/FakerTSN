#ifndef _TSN_CONFIGURATOR_H
#define _TSN_CONFIGURATOR_H

#include <string>
#include <sstream>

#include "INode.h"
#include "../utils/config/ConfigSetting.h"
#include "../utils/Log.h"
#include "../core/TSNContext.h"
#include "../core/InitTSNContextState.h"
#include "../core/RunTSNContextState.h"

namespace faker_tsn
{

class TSNConfigurator : public INode
{
private:
    std::string m_deviceName;

public:
    TSNConfigurator();

    virtual ~TSNConfigurator() override;

    virtual void run() override;

    virtual std::string toString() override {
        std::stringstream ss;
        ss << "[" << this->m_deviceName << "]";
        return ss.str();
    }
};

    
} // namespace faker_tsn

#endif // _TSN_CONFIGURATOR_H