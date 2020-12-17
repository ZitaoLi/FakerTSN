#include "RunTSNContextState.h"

#include "TSNContext.h"

namespace faker_tsn {

RunTSNContextState::RunTSNContextState(/* args */) {
}

RunTSNContextState::~RunTSNContextState() {
}

void RunTSNContextState::doAction(TSNContext& context) {
    auto prt = shared_from_this();
    context.setState(prt);
    INFO("RUN STATE");

    /* enable reactor */
    Reactor::getInstance().handle_events(); 
}

}  // namespace faker_tsn
