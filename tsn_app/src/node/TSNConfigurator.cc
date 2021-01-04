#include "TSNConfigurator.h"

namespace faker_tsn
{

TSNConfigurator::TSNConfigurator() {
    this->m_deviceName = ConfigSetting::getInstance().get<std::string>("deviceName");
    INFO(this->toString() + " Construct TSNConfigurator");
}

TSNConfigurator::~TSNConfigurator() {
    INFO(this->toString() + " Destruct TSNConfigurator");
}

void TSNConfigurator::run() {
    /* create tsn context */
    TSNContext& context = TSNContext::getInstance();

    /* change tsn context state to init */
    INFO("TSN_CONTEXT_STATE: null -> init");
    std::shared_ptr<InitTSNContextState> initState = std::make_shared<InitTSNContextState>();
    initState->doAction(context);

    /* change tsn context state to run */
    INFO("TSN_CONTEXT_STATE: init -> run");
    std::shared_ptr<ITSNContextState> runState = std::make_shared<RunTSNContextState>();
    runState->doAction(TSNContext::getInstance());
}
    
} // namespace faker_tsn
