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

    /* create port manager */
    std::shared_ptr<PortManager> manager = std::make_shared<PortManager>();
    context.setPortManager(manager);

    /* initialize mac table */
    MacTable::loadRouteXML("./config/routes.xml");
    INFO("----- MAC TABLE (unicast) -----\n" + MacTable::toString());
    MulticastMacTable::loadRouteXML("./config/routes.xml");
    INFO("----- MAC TABLE (multicast) -----\n" + MulticastMacTable::toString());

    /* enable reactor */
    Reactor::getInstance().handle_events(); 
}

}  // namespace faker_tsn
