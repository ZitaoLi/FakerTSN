#include "InitTSNContextState.h"

#include "TSNContext.h"
#include "RunTSNContextState.h"
#include "../utils/config/ConfigSetting.h"
#include "../networking/interface/MacAddress.h"
#include "./forward/MulticastMacTable.h"

namespace faker_tsn {

InitTSNContextState::InitTSNContextState(/* args */) {
}

InitTSNContextState::~InitTSNContextState() {
}

void InitTSNContextState::doAction(TSNContext& context) {
    auto prt = shared_from_this();
    context.setState(prt);
    INFO("START STATE");

    /* initilize ports */
    const char* infsc = ConfigSetting::getInstance().get<const char*>("switch.port.infs");
    std::vector<const char*> interfaces;
    char* _t = strtok(const_cast<char*>(infsc), " ");
    while (_t) {
        INFO("add interface [" + std::string(_t) + "]");
        interfaces.push_back(_t);
        _t = strtok(NULL, " ");
    }
    auto manager = context.getPortManager();
    for (auto interface: interfaces)
        manager->appendDeviceName(interface);
    manager->createPortFromDeviceNameList();

    /* initialize mac table */
    MacTable::loadRouteXML("./config/routes.xml");
    INFO("----- MAC TABLE (unicast) -----" + MacTable::toString());
    MulticastMacTable::loadRouteXML("./config/routes.xml");
    INFO("----- MAC TABLE (multicast) -----" + MulticastMacTable::toString());

    /* change state to run */
    std::shared_ptr<ITSNContextState> runState = std::make_shared<RunTSNContextState>();
    runState->doAction(context);
}

}  // namespace faker_tsn
