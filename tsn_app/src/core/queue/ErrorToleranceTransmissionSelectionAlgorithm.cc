#include "ErrorToleranceTransmissionSelectionAlgorithm.h"

#include "../TSNContext.h"
#include "../../networking/frame/EnhancementTSNFrameBody.h"

namespace faker_tsn
{

ErrorToleranceTransmissionSelectionAlgorithm::ErrorToleranceTransmissionSelectionAlgorithm(std::shared_ptr<InnerBuffer> innerBuffer) : TransmissionSelectionAlgorithm(innerBuffer) {
    INFO(this->toString() + " construct ErrorToleranceTransmissionSelectionAlgorithm");
}

ErrorToleranceTransmissionSelectionAlgorithm::~ErrorToleranceTransmissionSelectionAlgorithm() {
}

IFrameBody* ErrorToleranceTransmissionSelectionAlgorithm::dequeue() {
    if (this->m_egcl == nullptr) {
        std::shared_ptr<DataPort> port = std::dynamic_pointer_cast<DataPort>(TSNContext::getInstance().getPortManager()->getPort(this->m_innerBuffer->getPortIndex()));
        this->m_egcl = std::dynamic_pointer_cast<EnhancedGateControlList>(port->getGCL());
    }

    unsigned long cursor = 0;
    EnhancementTSNFrameBody* frame;
    frame = dynamic_cast<EnhancementTSNFrameBody*>(this->m_innerBuffer->getBy(cursor));
    while (frame != nullptr) {
        unsigned long flowId = frame->getFlowId();
        unsigned int phase = frame->getPhase();
        EnhancedGateControlListItem* item = (EnhancedGateControlListItem*)this->m_egcl->getCurrentItem();
        INFO(this->toString() + this->m_egcl->toString());
        INFO(this->toString() + " current item: " + item->toString());
        // is match?
        if (flowId == item->m_flowId && phase == item->m_phase) {
            INFO(this->toString() + " match frame: " + item->toString());
            return this->m_innerBuffer->dequeue();
        }
        // is eraly or late?
        if (this->isEarly(flowId, phase)) {
            INFO(this->toString() + " early-frame");
            cursor += 1;
            frame = dynamic_cast<EnhancementTSNFrameBody*>(this->m_innerBuffer->getBy(cursor));
        } else {
            INFO(this->toString() + " late-frame");
            this->m_innerBuffer->removeBy(cursor);
            break;
        }
    }
    return nullptr;
}

bool ErrorToleranceTransmissionSelectionAlgorithm::isEarly(unsigned long fid, unsigned int phs) {
    unsigned int gclSize = this->m_egcl->getGCLSize();
    unsigned int currentIndex = this->m_egcl->getCurrentIndex();
    for (int i = currentIndex - 1; i >= 0; i--) {
        EnhancedGateControlListItem* _item = (EnhancedGateControlListItem*)this->m_egcl->getItem(i);
        if (fid == _item->m_flowId && phs == _item->m_phase)
            return true;
    }
    return false;
}
    
} // namespace faker_tsn
