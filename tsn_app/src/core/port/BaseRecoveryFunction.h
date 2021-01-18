#ifndef _BASE_RECOVERY_FUNCTION
#define _BASE_RECOVERY_FUNCTION

#include <vector>
#include <list>
#include <unordered_map>
#include <sstream>

#include "../../networking/frame/EnhancementTSNFrameBody.h"
#include "../../utils/Log.h"
#include "../../utils/config/ConfigSetting.h"
#include "../../utils/monitor/DataSpot.h"

namespace faker_tsn
{

struct FrameRecordEntry {
    int lower;
    int upper;
};

typedef std::list<FrameRecordEntry> frame_recorder;

class BaseRecoveryFunction {
private:
    std::string m_deviceName;
    uint8_t m_portId;
    std::unordered_map<uint32_t, frame_recorder> m_flowSeqMap;

private:
    bool checkRepetitive(frame_recorder& fr, uint16_t seq);

public:
    BaseRecoveryFunction() = default;

    BaseRecoveryFunction(uint8_t portId) : m_portId(portId) {
        this->m_deviceName = ConfigSetting::getInstance().get<std::string>("deviceName");
    }

    ~BaseRecoveryFunction() = default;

    std::string toString() {
        std::stringstream ss;
        ss << "[" << this->m_deviceName << "]";
        ss << "[" << this->m_portId << "]";
        return ss.str();
    }

    /* handle packet */
    IFrameBody* handle(IFrameBody* frame);
};

} // namespace faker_tsn


#endif // _BASE_RECOVERY_FUNCTION