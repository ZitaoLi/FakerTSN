#include "TransmissionSelection.h"

#include "QueueContext.h"

namespace faker_tsn {

// TransmissionSelection::TransmissionSelection(std::vector<std::shared_ptr<IQueue>> queues) {
//     this->m_queues = queues;
// }

IFrameBody* TransmissionSelection::dequeue(QueueContext& queueContext) {
    INFO(queueContext.m_deviceName + ".port" + std::to_string(queueContext.m_portIndex) + " TransmissionSelection on call");
    /* invoke queueu from 7 to 0 */
    for (auto it = queueContext.m_queues.rbegin(); it != queueContext.m_queues.rend(); it++) {
        IFrameBody* frame = (*it)->dequeue();
        if (frame != nullptr)
            return frame;
    }
    return nullptr;
}

}  // namespace faker_tsn
