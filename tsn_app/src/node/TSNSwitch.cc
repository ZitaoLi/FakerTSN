#include "TSNSwitch.h"

namespace faker_tsn
{

TSNSwitch::TSNSwitch() {

}

TSNSwitch::~TSNSwitch() {

}

void TSNSwitch::run() {
    /* create tsn context */
    TSNContext& context = TSNContext::getInstance();

    /* change tsn context state to init */
    INFO("TSN_CONTEXT_STATE: null -> init");
    std::shared_ptr<InitTSNContextState> initState = std::make_shared<InitTSNContextState>();
    initState->doAction(context);
}
    
} // namespace faker_tsn
