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

class GateControlListItem {
   public:
    Time::TimeInterval m_timeInterval; /* time interval */
    std::bitset<8> m_gateStates;       /* gate states */

    GateControlListItem(Time::TimeInterval timeInterval, std::bitset<8> gateStates = std::bitset<8>("00000000")) : m_timeInterval(timeInterval), m_gateStates(gateStates) {}

    std::string toString() {
        std::stringstream ss;
        ss << "TimeInterval(" << this->m_timeInterval.toString() << ") ";
        ss << "GateStates(" << this->m_gateStates.to_string() << ")";
        return ss.str();
    }
};

class TransmissionGate;

class GateControlList : public REFLECT_OBJECT, public DynamicCreator<GateControlList, unsigned int> {
   private:
    unsigned int m_length;                                  /* length of CGL */
    unsigned int m_cursor;                                  /* pointer of current item */
    unsigned int m_portId;
    unsigned int m_gateSize;                                /* no. of gates */
    std::vector<std::shared_ptr<TransmissionGate>> m_gates; /* gate container */
    std::vector<GateControlListItem> m_gcl;                 /* gate control list (GCL) */
    Time::TimeInterval m_period;                            /* scheduling period */

    /* load GCL XML file */
    virtual void loadScheduleXML(std::string filename);

   public:
    GateControlList(unsigned int portId);

    virtual ~GateControlList() = default;

    /* get port index */
    inline unsigned int getPortId() {
        return this->m_portId;
    }

    /* get current item */
    GateControlListItem getCurrentItem();

    std::string toString();

    /*** Obeserver Pattern ***/

    /* append gate */
    void appendGate(std::shared_ptr<TransmissionGate> gate);

    /* update all gates */
    void updateGates();
};

}  // namespace faker_tsn

#endif  // _GATE_CONTROL_LIST_H