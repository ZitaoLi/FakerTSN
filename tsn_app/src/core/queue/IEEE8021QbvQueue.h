#ifndef _IEEE_8021_QBV_QUEUE_H
#define _IEEE_8021_QBV_QUEUE_H

#include <memory>
#include <string>

#include "../../utils/Log.h"
#include "../../utils/reflector/DynamicCreate.h"
#include "IQueue.h"
#include "InnerBuffer.h"
#include "PriorityStrictTransmissionSelectionAlgorithm.h"
#include "ErrorToleranceTransmissionSelectionAlgorithm.h"
#include "IEEE8021Queue.h"
#include "TransmissionGate.h"

namespace faker_tsn
{

class IEEE8021QbvQueue : public IQueue, public REFLECT_OBJECT, public DynamicCreator<IEEE8021QbvQueue, unsigned short, uint8_t>
{
private:
    std::string m_deviceName;
    unsigned short m_portIndex;
    uint8_t m_pcp;
    std::shared_ptr<InnerBuffer>
        m_innerBuffer; /* inner buffer */
    std::shared_ptr<TransmissionSelectionAlgorithm>
        m_transmissionSelectionAlgorithm; /* transmission selection algorithm */
    std::shared_ptr<TransmissionGate> m_transmissionGate;

public:
    IEEE8021QbvQueue(unsigned short portIndex, uint8_t pcp);

    ~IEEE8021QbvQueue() override;

    /* get pcp */
    uint8_t getPCP() {
        return this->m_pcp;
    }

    /* get pcp */
    uint8_t getPortIndex() {
        return this->m_portIndex;
    }

    /* get device name */
    std::string getDeviceName() {
        return this->m_deviceName;
    }

    /* get innner buffer */
    std::shared_ptr<InnerBuffer> getInnerBuffer() {
        return this->m_innerBuffer;
    }

    /* get transmission algorithm */
    std::shared_ptr<TransmissionSelectionAlgorithm> getTransmissionSelectionAlgorithm() {
        return this->m_transmissionSelectionAlgorithm;
    }

    /* get transmission gate */
    std::shared_ptr<TransmissionGate> getTransmissionGate() {
        return this->m_transmissionGate;
    }

    /* enqueue frame body */
    virtual void enqueue(IFrameBody* frameBody) override;

    /* get frame body from queue */
    virtual IFrameBody* dequeue() override;

    /* check whether queue is emptry or not */
    virtual bool isEmpty() override;

    /* get capacity of queue */
    virtual unsigned int getCapacity() override;

    /* get residual capacity of queue */
    virtual unsigned int getResidualCapacity() override;
};
    
} // namespace faker_tsn


#endif // _IEEE_8021_QBV_QUEUE_H