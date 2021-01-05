#ifndef _ENHANCED_GATE_CONTROL_LIST_H
#define _ENHANCED_GATE_CONTROL_LIST_H

#include <bitset>
#include <string>
#include <sstream>
#include <memory>
#include <vector>

#include "../../timer/ITimer.h"
#include "../../utils/reflector/DynamicCreate.h"
#include "GateControlList.h"

namespace faker_tsn
{

class EnhancedGateControlListItem : public GateControlListItem {
public:
    unsigned int m_flowId;
    unsigned int m_phase;

    EnhancedGateControlListItem(
        Time::TimeInterval  timeInterval,
        std::bitset<8>      gateStates,
        unsigned int        flowId,
        unsigned int        phase) : 
        GateControlListItem(timeInterval, gateStates),
        m_flowId(           flowId),
        m_phase(            phase) {
    }

    std::string toString() {
        std::stringstream ss;
        ss << "length[" << m_timeInterval.toString() << "] ";
        ss << "states[" << m_gateStates.to_string() << "] ";
        ss << "flowid[" << std::to_string(m_flowId) << "] ";
        ss << "phase[" << std::to_string(m_phase) << "] ";
        return ss.str();
    }
};

class EnhancedGateControlList : 
    public TimeAwareShaper,
    public REFLECT_OBJECT, 
    public DynamicCreator<EnhancedGateControlList, unsigned int> {
private:
    unsigned int m_length;
    unsigned int m_cursor;
    unsigned int m_portId;
    unsigned int m_gateSize;
    Time::TimeInterval m_period;
    std::vector<std::shared_ptr<TransmissionGate>> m_gates;
    std::vector<EnhancedGateControlListItem*> m_gcl;

private:
    void loadScheduleXML(std::string filename);

public:
    EnhancedGateControlList(unsigned int portId);

    ~EnhancedGateControlList();

    virtual std::string toString() override;

    virtual void registerGCLfromSchedules() override;

    virtual unsigned int getPortId() override {
        return this->m_portId;
    }

    virtual GateControlListItem* getCurrentItem() override;

    virtual void appendGate(std::shared_ptr<TransmissionGate> gate) override;

    virtual void updateGates() override;
};
    
} // namespace faker_tsn


#endif // _ENHANCED_GATE_CONTROL_LIST_H