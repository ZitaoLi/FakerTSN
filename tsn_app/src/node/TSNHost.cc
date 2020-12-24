#include "TSNHost.h"

namespace faker_tsn
{

TSNHost::TSNHost() {
    this->m_deviceName = ConfigSetting::getInstance().get<std::string>("deviceName");
    INFO(this->toString() + " Construct TSNHost");
}

TSNHost::~TSNHost() {
    INFO(this->toString() + " Destruct TSNHost");
}

void TSNHost::run() {
    /* create tsn context */
    TSNContext& context = TSNContext::getInstance();

    /* change tsn context state to init */
    INFO("TSN_CONTEXT_STATE: null -> init");
    std::shared_ptr<InitTSNContextState> initState = std::make_shared<InitTSNContextState>();
    initState->doAction(context);
}
    
} // namespace faker_tsn
