#include "GateControlTicker.h"
#include "../../utils/config/ConfigSetting.h"
#include "../../utils/reflector/Reflector.h"

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

    /* set a new ticker */
    GateControlListItem item = this->m_gcl->getCurrentItem();
    Time::TimePoint nextStartTime(0, 0);
    // current clock time
    Time::TimePoint nowTime = Reflector::getNewInstance<IClock>("RealTimeClock")->now();
    INFO("[" + std::to_string(this->m_gcl->getPortId()) + "] now\t\t " + nowTime.toString());
    // created clocl time
    Time::TimePoint launchTime =  this->getLanunchTime();
    // real interval
    Time::TimeInterval interval1 = nowTime - launchTime;
    INFO("[" + std::to_string(this->m_gcl->getPortId()) + "] interval1\t " + interval1.toString());
    // deviation = real interval - ideal interval
    Time::TimeInterval deviation = interval1 - this->getExpire();
    INFO("[" + std::to_string(this->m_gcl->getPortId()) + "] deviation\t " + deviation.toString());
    // precision
    Time::TimeInterval precision = Time::converIntegerToTimeInterval(TimeContext::getInstance().getTimer()->getPrecision(), "ns");
    INFO("[" + std::to_string(this->m_gcl->getPortId()) + "] precision\t " + interval1.toString());
    if (deviation >= precision) {
        ERROR("[" + std::to_string(this->m_gcl->getPortId()) + "] out of precision");
        exit(EXIT_FAILURE);
    }
    // next ideal interval
    Time::TimeInterval interval2 = item.m_timeInterval - deviation;
    INFO("[" + std::to_string(this->m_gcl->getPortId()) + "] interval2 " + interval1.toString());
    std::shared_ptr<Ticker> ticker = std::make_shared<GateControlTicker>(
        nextStartTime,
        interval2,
        this->m_gcl);

    /* refresh gates */
    this->m_gcl->updateGates();
}
    
} // namespace faker_tsn
