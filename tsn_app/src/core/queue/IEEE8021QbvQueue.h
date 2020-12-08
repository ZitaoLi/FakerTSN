#ifndef _IEEE_8021_QBV_QUEUE_H
#define _IEEE_8021_QBV_QUEUE_H

#include <memory>

#include "IEEE8021Queue.h"
#include "TransmissionGate.h"

namespace faker_tsn
{

class IEEE8021QbvQueue : public IEEE8021Queue, public REFLECT_OBJECT, public DynamicCreator<IEEE8021Queue, unsigned short, uint8_t>
{
private:
    std::shared_ptr<TransmissionGate> m_gate;

public:
    IEEE8021QbvQueue(unsigned short portIndex, uint8_t pcp);

    ~IEEE8021QbvQueue() override;

    // /* get frame body from queue */
    // virtual IFrameBody* dequeue() override;
};
    
} // namespace faker_tsn


#endif // _IEEE_8021_QBV_QUEUE_H