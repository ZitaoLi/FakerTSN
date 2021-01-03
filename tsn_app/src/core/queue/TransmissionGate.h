#ifndef _TRANSMISSON_GATE_H
#define _TRANSMISSON_GATE_H

#include <cstdint>
#include <memory>

#include "../../networking/frame/IFrameBody.h"
#include "TransmissionSelectionAlgorithm.h"
#include "GateControlList.h"

namespace faker_tsn {

class TransmissionGate : public std::enable_shared_from_this<TransmissionGate> {
   private:
    uint8_t m_pcp;
    bool m_isOpen;
    std::shared_ptr<TransmissionSelectionAlgorithm> m_algorithm;

   public:
    TransmissionGate() : m_isOpen(true), m_algorithm(nullptr), m_pcp(0) {}
    
    TransmissionGate(std::shared_ptr<TransmissionSelectionAlgorithm> algorithm) : m_isOpen(true) {
        this->m_algorithm = algorithm;
        this->m_pcp = algorithm->getPCP();
    }

    ~TransmissionGate() = default;

    inline uint8_t getPCP() {
        return this->m_pcp;
    }

    inline void setOpen(bool state) {
        this->m_isOpen = state;
    }

    inline bool isOpen() {
        return this->m_isOpen;
    }

    inline std::shared_ptr<TransmissionSelectionAlgorithm> getTransmissionSelectionAlogirthm() {
        return this->m_algorithm;
    }

    /* dequeue head frame */
    virtual IFrameBody* dequeue();

    /* Observer pattern */

    /* register into GateControlList */
    void registerGCL(std::shared_ptr<GateControlList> gcl);

    /* callable funtion */
    void onUpdate(uint8_t pcp, bool isOpen);
};

}  // namespace faker_tsn

#endif  // _TRANSMISSON_GATE_H