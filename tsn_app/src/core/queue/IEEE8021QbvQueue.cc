#include "IEEE8021QbvQueue.h"

namespace faker_tsn
{

IEEE8021QbvQueue::IEEE8021QbvQueue(unsigned short portIndex, uint8_t pcp) : IEEE8021Queue(portIndex, pcp) {
    /* initialize transmission gate */
    this->m_gate = std::make_shared<TransmissionGate>(this->getTransmissionSelectionAlgorithm());
}

IEEE8021QbvQueue::~IEEE8021QbvQueue() {};

// TODO
// IFrameBody* IEEE8021QbvQueue::dequeue() {
//     INFO(this->getDeviceName() + ".port" + std::to_string(this->getPortIndex()) + ".queue" + std::to_string(this->getPCP()) + " dequeue");

//     return nullptr;
// }
    
} // namespace faker_tsn
