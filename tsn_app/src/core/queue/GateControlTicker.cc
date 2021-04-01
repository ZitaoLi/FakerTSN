#include "GateControlTicker.h"
#include "../../utils/config/ConfigSetting.h"
#include "../../utils/reflector/Reflector.h"
#include "../../networking/Reactor.h"
#include "../../core/TSNContext.h"

namespace faker_tsn
{

GateControlTicker::GateControlTicker(Time::TimePoint& start, Time::TimeInterval& expire, Time::TimeInterval& period) : Ticker(start, expire, period), m_gcl(nullptr) {}

GateControlTicker::GateControlTicker(Time::TimePoint& start, Time::TimeInterval& expire) : Ticker(start, expire), m_gcl(nullptr) {}

GateControlTicker::GateControlTicker(Time::TimePoint& start, Time::TimeInterval& expire, Time::TimeInterval& period, TimeAwareShaper* gcl) : Ticker(start, expire, period), m_gcl(gcl) {}

GateControlTicker::GateControlTicker(Time::TimePoint& start, Time::TimeInterval& expire, TimeAwareShaper* gcl) : Ticker(start, expire), m_gcl(gcl) {}

void GateControlTicker::operator()() {
    INFO("oncall GateControlTicker");
    if (!this->m_gcl) {
        ERROR("GCL not be setted");
        exit(EXIT_FAILURE);
    }

    /* refresh gates */
    this->m_gcl->updateGates();

    /* set a new ticker */
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
    INFO("[" + std::to_string(this->m_gcl->getPortId()) + "] precision\t " + precision.toString());
    
    // print deviation
    struct timeval tp;
    gettimeofday(&tp, NULL);
    long int ms = tp.tv_sec * 1000 + tp.tv_usec / 1000;
    std::string cmd = "echo \"" + std::to_string(ms) + "," + std::to_string(deviation.nsec) + "\">> ./config/simulation_0/error.log";
    system(cmd.c_str());

    if (deviation >= precision) {
        ERROR("[" + std::to_string(this->m_gcl->getPortId()) + "] out of precision: " + deviation.toString());
        // TODO record
    }
    // next ideal interval
    GateControlListItem* item = this->m_gcl->getNextItem();
    Time::TimePoint nextStartTime(0, 0);
    Time::TimeInterval interval2 = item->m_timeInterval - deviation;
    INFO("[" + std::to_string(this->m_gcl->getPortId()) + "] interval2 " + interval2.toString());
    Ticker* ticker = new GateControlTicker(
        nextStartTime,
        interval2,
        this->m_gcl);
    TimeContext::getInstance().getTimer()->addTicker(ticker);

    /* enable timer */
    TimeContext::getInstance().getTimer()->start();

    /* enable EPOLLOUT event */
    auto port = TSNContext::getInstance().getPortManager()->getPort(this->m_gcl->getPortId());
    Reactor::getInstance().getDemultoplexer().updateHandle(port->getOutSockfd(), EPOLLOUT);
}
    
} // namespace faker_tsn
