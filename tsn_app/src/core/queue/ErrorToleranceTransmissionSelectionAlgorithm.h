#ifndef _ERROR_TOLERANCE_TRANSMISSION_SELECTION_ALGORITHM
#define _ERROR_TOLERANCE_TRANSMISSION_SELECTION_ALGORITHM

#include "TransmissionSelectionAlgorithm.h"

namespace faker_tsn
{

class ErrorToleranceTransmissionSelectionAlgorithm : public TransmissionSelectionAlgorithm {
public:
    ErrorToleranceTransmissionSelectionAlgorithm();

    virtual ~ErrorToleranceTransmissionSelectionAlgorithm() override;

    virtual IFrameBody* dequeue() override;
};
    
} // namespace faker_tsn


#endif // _ERROR_TOLERANCE_TRANSMISSION_SELECTION_ALGORITHM