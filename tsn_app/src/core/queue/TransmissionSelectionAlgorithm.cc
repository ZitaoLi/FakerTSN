#include "TransmissionSelectionAlgorithm.h"

namespace faker_tsn {

TransmissionSelectionAlgorithm::TransmissionSelectionAlgorithm(std::shared_ptr<InnerBuffer> innerBuffer) {
    this->m_innerBuffer = innerBuffer;
    this->m_pcp = innerBuffer->getPCP();
}

TransmissionSelectionAlgorithm::~TransmissionSelectionAlgorithm() {
}

IFrameBody* TransmissionSelectionAlgorithm::dequeue() {
    IFrameBody* frame = this->m_innerBuffer->dequeue();
    if (frame != nullptr)
        return frame;
    return nullptr;
}

}  // namespace faker_tsn
