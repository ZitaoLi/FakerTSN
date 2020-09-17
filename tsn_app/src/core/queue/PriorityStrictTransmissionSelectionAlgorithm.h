#ifndef _PRIORITY_STRICT_TRANSMISSION_SELECTION_ALGORITHM
#define _PRIORITY_STRICT_TRANSMISSION_SELECTION_ALGORITHM

#include "../../utils/reflector/DynamicCreate.h"
#include "TransmissionSelectionAlgorithm.h"

namespace faker_tsn {

class PriorityStrictTransmissionSelectionAlgorithm : public TransmissionSelectionAlgorithm, public REFLECT_OBJECT, public DynamicCreator<PriorityStrictTransmissionSelectionAlgorithm, std::shared_ptr<InnerBuffer>> {
   public:
    PriorityStrictTransmissionSelectionAlgorithm(std::shared_ptr<InnerBuffer> innerBuffer) : TransmissionSelectionAlgorithm(innerBuffer) {}

    ~PriorityStrictTransmissionSelectionAlgorithm();
};

}  // namespace faker_tsn

#endif  // _PRIORITY_STRICT_TRANSMISSION_SELECTION_ALGORITHM