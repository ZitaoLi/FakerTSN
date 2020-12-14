#include "TransmissionGate.h"

namespace faker_tsn
{

IFrameBody* TransmissionGate::dequeue() {
    if (this->m_isOpen)
        return this->m_algorithm->dequeue();

    return nullptr;
}

void TransmissionGate::registerGCL(std::shared_ptr<GateControlList> gcl) {
    gcl->appendGate(shared_from_this());
}

void TransmissionGate::onUpdate(uint8_t pcp, bool isOpen) {
    if (pcp != this->m_pcp)
        ERROR("pcp not matched");

    this->m_isOpen = isOpen;
}
    
} // namespace faker_tsn
