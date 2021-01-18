#include "ErrorToleranceTransmissionSelectionAlgorithm.h"

#include "../TSNContext.h"
#include "../../networking/frame/EnhancementTSNFrameBody.h"
#include "../port/DataPort.h"

namespace faker_tsn
{

ErrorToleranceTransmissionSelectionAlgorithm::ErrorToleranceTransmissionSelectionAlgorithm(std::shared_ptr<InnerBuffer> innerBuffer) : TransmissionSelectionAlgorithm(innerBuffer) {
    INFO(this->toString() + " construct ErrorToleranceTransmissionSelectionAlgorithm");
}

ErrorToleranceTransmissionSelectionAlgorithm::~ErrorToleranceTransmissionSelectionAlgorithm() {
}

IFrameBody* ErrorToleranceTransmissionSelectionAlgorithm::dequeue() {
    if (this->m_egcl == nullptr) {
        this->m_port = std::dynamic_pointer_cast<DataPort>(TSNContext::getInstance().getPortManager()->getPort(this->m_innerBuffer->getPortIndex()));
        this->m_egcl = std::dynamic_pointer_cast<EnhancedGateControlList>(this->m_port->getGCL());
    }

    unsigned long cursor = 0;
    EnhancementTSNFrameBody* frame;
    frame = dynamic_cast<EnhancementTSNFrameBody*>(this->m_innerBuffer->getBy(cursor));
    while (frame != nullptr) {
        unsigned long flowId = frame->getFlowId();
        unsigned int phase = frame->getPhase();
        EnhancedGateControlListItem* item = (EnhancedGateControlListItem*)this->m_egcl->getCurrentItem();
        INFO(this->toString() + " EGCL: " + this->m_egcl->toString());
        INFO(this->toString() + " current item: " + item->toString());
        INFO(this->toString() + " current frame: " + frame->toString());
        // is match?
        if (flowId == item->m_flowId && phase == item->m_phase) {
            INFO(this->toString() + " match frame: " + item->toString());
            // this->m_port->setClosedOutEvent(false);
            return this->m_innerBuffer->dequeue();
        }
        // is eraly or late?
        if (this->isEarly(flowId, phase)) {
            INFO(this->toString() + " early-frame");
            cursor += 1;
        } else {
            INFO(this->toString() + " late-frame");
            this->m_innerBuffer->removeBy(cursor);
        }
        frame = dynamic_cast<EnhancementTSNFrameBody*>(this->m_innerBuffer->getBy(cursor));
    }
    // this->m_port->setClosedOutEvent(true);
    return nullptr;
}

bool ErrorToleranceTransmissionSelectionAlgorithm::isEarly(unsigned long fid, unsigned int phs) {
    unsigned int gclSize = this->m_egcl->getGCLSize();
    unsigned int currentIndex = this->m_egcl->getCurrentIndex();
    for (int i = currentIndex - 1; i >= 0; i--) {
        EnhancedGateControlListItem* _item = (EnhancedGateControlListItem*)this->m_egcl->getItem(i);
        if (fid == _item->m_flowId && phs == _item->m_phase)
            return false;
    }
    return true;
}
    
} // namespace faker_tsn
