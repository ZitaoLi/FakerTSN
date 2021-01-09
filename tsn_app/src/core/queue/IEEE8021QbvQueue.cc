#include "IEEE8021QbvQueue.h"

#include "../../utils/config/ConfigSetting.h"

namespace faker_tsn
{

IEEE8021QbvQueue::IEEE8021QbvQueue(unsigned short portIndex, uint8_t pcp) : m_portIndex(portIndex), m_pcp(pcp) {
    ConfigSetting& cs = ConfigSetting::getInstance();
    this->m_deviceName = cs.get<std::string>("deviceName");
    INFO("Construct " + this->m_deviceName + ".port" + std::to_string(this->m_portIndex) + ".queue" + std::to_string(this->m_pcp));

    /* initialize innner buffer */
    int capacity = cs.get<int>("switch.queue.capacity");
    this->m_innerBuffer = std::make_shared<InnerBuffer>(this->m_portIndex, this->m_pcp, capacity);

    /* initialize transimssion selection algorithm */
    std::string transmissionSelectionAlgorithmClass = cs.get<std::string>("switch.queue.transmissionSelectionAlgorithm");
    if (transmissionSelectionAlgorithmClass == "PriorityStrictTransmissionSelectionAlgorithm") {
        this->m_transmissionSelectionAlgorithm = std::make_shared<PriorityStrictTransmissionSelectionAlgorithm>(this->m_innerBuffer);
    } else if (transmissionSelectionAlgorithmClass == "ErrorToleranceTransmissionSelectionAlgorithm") {
        this->m_transmissionSelectionAlgorithm = std::make_shared<ErrorToleranceTransmissionSelectionAlgorithm>(this->m_innerBuffer);
    } else {
        ERROR("invalid transmissionSelectionAlgorithmClass");
        exit(EXIT_FAILURE);
    }

    /* initialize transmission gate */
    this->m_transmissionGate = std::make_shared<TransmissionGate>(this->getTransmissionSelectionAlgorithm());
}

IEEE8021QbvQueue::~IEEE8021QbvQueue() {};

/* enqueue frame body */
void IEEE8021QbvQueue::enqueue(IFrameBody* frameBody) {
    INFO(this->m_deviceName + ".port" + std::to_string(this->m_portIndex) + ".queue" + std::to_string(this->m_pcp) + " enqueue");

    this->m_innerBuffer->enqueue(frameBody);
}

/* get frame body from queue */
IFrameBody* IEEE8021QbvQueue::dequeue() {
    INFO(this->m_deviceName + ".port" + std::to_string(this->m_portIndex) + ".queue" + std::to_string(this->m_pcp) + " dequeue, gate stats = " + std::to_string(this->m_transmissionGate->isOpen()));

    return this->m_transmissionGate->dequeue();
}

bool IEEE8021QbvQueue::isEmpty() {
    return this->m_innerBuffer->isEmpty();
}

unsigned int IEEE8021QbvQueue::getCapacity() {
    return this->m_innerBuffer->getCapacity();
}

unsigned int IEEE8021QbvQueue::getResidualCapacity() {
    return this->m_innerBuffer->getResidualCapacity();
}
    
} // namespace faker_tsn
