#ifndef _ERROR_TOLERANCE_TRANSMISSION_SELECTION_ALGORITHM
#define _ERROR_TOLERANCE_TRANSMISSION_SELECTION_ALGORITHM

#include <memory>

#include "TransmissionSelectionAlgorithm.h"
#include "InnerBuffer.h"
#include "EnhancedGateControlList.h"
#include "../../utils/reflector/DynamicCreate.h"

namespace faker_tsn
{

class ErrorToleranceTransmissionSelectionAlgorithm : 
    public TransmissionSelectionAlgorithm,
    public REFLECT_OBJECT, 
    public DynamicCreator<
        ErrorToleranceTransmissionSelectionAlgorithm, 
        std::shared_ptr<InnerBuffer>> {
private:
    std::shared_ptr<EnhancedGateControlList> m_egcl;

    bool isEarly(unsigned long fid, unsigned int phs);
    
public:
    ErrorToleranceTransmissionSelectionAlgorithm(std::shared_ptr<InnerBuffer> innerBuffer);

    virtual ~ErrorToleranceTransmissionSelectionAlgorithm() override;

    virtual IFrameBody* dequeue() override;
};
    
} // namespace faker_tsn


#endif // _ERROR_TOLERANCE_TRANSMISSION_SELECTION_ALGORITHM