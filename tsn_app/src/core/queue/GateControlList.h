#ifndef _GATE_CONTROL_LIST_H
#define _GATE_CONTROL_LIST_H

#include <tinyxml2/tinyxml2.h>

#include <bitset>
#include <exception>
#include <memory>
#include <string>
#include <sstream>
#include <vector>

#include "../../timer/ITimer.h"
#include "../../timer/TimeContext.h"
#include "../../utils/Log.h"
#include "../../utils/config/ConfigSetting.h"
// #include "../../utils/reflector/Reflector.h"
#include "../../utils/reflector/DynamicCreate.h"

using namespace tinyxml2;

namespace faker_tsn {

class TransmissionGate;

class GateControlListItem {
   public:
    Time::TimeInterval m_timeInterval; /* time interval */
    std::bitset<8> m_gateStates;       /* gate states */

    GateControlListItem(Time::TimeInterval timeInterval, std::bitset<8> gateStates ) : m_timeInterval(timeInterval), m_gateStates(std::bitset<8>("00000000")) {}

    std::string toString() {
        std::stringstream ss;
        ss << "TimeInterval(" << this->m_timeInterval.toString() << ") ";
        ss << "GateStates(" << this->m_gateStates.to_string() << ")";
        return ss.str();
    }
};

class TimeAwareShaper {
public:
    virtual ~TimeAwareShaper() = default;
    virtual std::string toString() = 0;
    /* get port index */
    virtual unsigned int getPortId() = 0;
    /* port use this function to register timer */
    virtual void registerGCLfromSchedules() = 0;
    /* get current gate control list item */
    virtual GateControlListItem* getCurrentItem() = 0;
    /*** Obeserver Pattern ***/
    virtual void appendGate(std::shared_ptr<TransmissionGate> gate) = 0;
    virtual void updateGates() = 0;
};

class GateControlList : 
    public TimeAwareShaper, 
    public REFLECT_OBJECT, 
    public DynamicCreator<GateControlList, unsigned int> {
   private:
    unsigned int m_length;                                  /* length of CGL */
    unsigned int m_cursor;                                  /* pointer of current item */
    unsigned int m_portId;
    unsigned int m_gateSize;                                /* no. of gates */
    std::vector<std::shared_ptr<TransmissionGate>> m_gates; /* gate container */
    std::vector<GateControlListItem*> m_gcl;                /* gate control list (GCL) */
    Time::TimeInterval m_period;                            /* scheduling period */

    /* load GCL XML file */
    void loadScheduleXML(std::string filename);

   public:
    GateControlList(unsigned int portId);

    virtual ~GateControlList() = default;

    virtual std::string toString() override;

    virtual void registerGCLfromSchedules() override;

    virtual unsigned int getPortId() override {
        return this->m_portId;
    }

    virtual GateControlListItem* getCurrentItem() override;

    virtual void appendGate(std::shared_ptr<TransmissionGate> gate) override;

    virtual void updateGates() override;
};

}  // namespace faker_tsn

#endif  // _GATE_CONTROL_LIST_H