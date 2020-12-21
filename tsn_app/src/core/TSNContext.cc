#include "TSNContext.h"

namespace faker_tsn {

TSNContext::TSNContext(/* args */) {
    // TODO create Timer
}

TSNContext::~TSNContext() {
}

void TSNContext::setState(const std::shared_ptr<ITSNContextState>& state) {
    this->m_state = state;
}

std::shared_ptr<ITSNContextState>& TSNContext::getState() {
    return this->m_state;
}

}  // namespace faker_tsn
