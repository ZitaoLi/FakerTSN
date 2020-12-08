#include "GateControlTicker.h"

namespace faker_tsn
{

GateControlTicker::GateControlTicker(Time::TimePoint& start, Time::TimeInterval& expire, Time::TimeInterval& period) : Ticker(start, expire, period), m_gcl(nullptr) {}

GateControlTicker::GateControlTicker(Time::TimePoint& start, Time::TimeInterval& expire) : Ticker(start, expire), m_gcl(nullptr) {}

GateControlTicker::GateControlTicker(Time::TimePoint& start, Time::TimeInterval& expire, Time::TimeInterval& period, GateControlList* gcl) : Ticker(start, expire, period), m_gcl(gcl) {}

GateControlTicker::GateControlTicker(Time::TimePoint& start, Time::TimeInterval& expire, GateControlList* gcl) : Ticker(start, expire), m_gcl(gcl) {}

void GateControlTicker::operator()() {
    INFO("oncall GateControlTicker");
    if (!this->m_gcl) {
        ERROR("GCL not be setted");
        exit(-1);
    }
    INFO(this->m_gcl->toString());
}
    
} // namespace faker_tsn
