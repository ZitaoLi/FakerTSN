#ifndef _TRANSMISSION_SELECTION_ALGORITHM_H
#define _TRANSMISSION_SELECTION_ALGORITHM_H

#include <cstdint>
#include <memory>
#include <sstream>
#include "../../networking/frame/IFrameBody.h"
#include "InnerBuffer.h"

namespace faker_tsn {

class TransmissionSelectionAlgorithm {
   protected:
    uint8_t m_pcp;
    std::shared_ptr<InnerBuffer> m_innerBuffer;

   public:
    TransmissionSelectionAlgorithm(std::shared_ptr<InnerBuffer> innerBuffer);
    virtual ~TransmissionSelectionAlgorithm();

    inline uint8_t getPCP() {
        return this->m_pcp;
    }

    virtual IFrameBody* dequeue();

    std::string toString() {
        std::stringstream ss;
        ss << "[" << std::to_string(this->m_innerBuffer->getPortIndex()) << "]";
        ss << "[" << std::to_string(this->m_innerBuffer->getPCP()) << "]";
        return ss.str();
    }
};

}  // namespace faker_tsn

#endif  // _TRANSMISSION_SELECTION_ALGORITHM_H