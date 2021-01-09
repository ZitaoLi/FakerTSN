#include "PriorityStrictTransmissionSelectionAlgorithm.h"

namespace faker_tsn
{

PriorityStrictTransmissionSelectionAlgorithm::PriorityStrictTransmissionSelectionAlgorithm(std::shared_ptr<InnerBuffer> innerBuffer) : TransmissionSelectionAlgorithm(innerBuffer) {
    INFO(this->toString() + " construct PriorityStrictTransmissionSelectionAlgorithm");
}

PriorityStrictTransmissionSelectionAlgorithm::~PriorityStrictTransmissionSelectionAlgorithm() {
}

IFrameBody* PriorityStrictTransmissionSelectionAlgorithm::dequeue() {
    return TransmissionSelectionAlgorithm::dequeue();
}
    
} // namespace faker_tsn
