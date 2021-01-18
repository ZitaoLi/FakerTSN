#include "FlowTicker.h"

namespace faker_tsn
{

FlowTicker::FlowTicker(Time::TimePoint& start, Time::TimeInterval& expire, Time::TimeInterval& period, FlowApp* flowApp) : Ticker(start, expire, period), m_flowApp(flowApp) {
}

FlowTicker::FlowTicker(Time::TimePoint& start, Time::TimeInterval& expire, FlowApp* flowApp) : Ticker(start, expire), m_flowApp(flowApp) {
}

FlowTicker::~FlowTicker() {
}

/* handler */
void FlowTicker::operator()() {
    INFO(this->m_flowApp->toString() + " oncall GateControlTicker");

    Flow* flow = this->m_flowApp->flows[this->m_flowApp->flowIndex];

    // current clock time
    Time::TimePoint nowTime = Reflector::getNewInstance<IClock>("RealTimeClock")->now();
    INFO("  now\t\t " + nowTime.toString());
    // launched clocl time
    Time::TimePoint launchTime =  this->getLanunchTime();
    // real interval
    Time::TimeInterval interval1 = nowTime - launchTime;
    INFO("  interval1\t " + interval1.toString());
    // deviation = real interval - ideal interval
    Time::TimeInterval deviation = interval1 - this->getExpire();
    INFO("  deviation\t " + deviation.toString());
    // precision
    Time::TimeInterval precision = Time::converIntegerToTimeInterval(TimeContext::getInstance().getTimer()->getPrecision(), "ns");
    INFO("  precision\t " + precision.toString());
    if (deviation >= precision) {
        ERROR(" out of precision: " + deviation.toString());
        // TODO record
    }
    // next ideal interval
    Time::TimePoint nextStartTime(0, 0);
    Time::TimeInterval interval2 = flow->periodInterval - deviation;
    INFO("  interval2\t " + interval2.toString());

    // add next ticker
    // Ticker* ticker = new FlowTicker(
    //     nextStartTime,
    //     interval2,
    //     this->m_flowApp
    // );
    // TimeContext::getInstance().getTimer()->addTicker(ticker);
    registerFlowToTimer(this->m_flowApp, flow, interval2);

    this->m_flowApp->flowIndex = (this->m_flowApp->flowIndex + 1) % this->m_flowApp->flowNum;

    /* enable timer */
    TimeContext::getInstance().getTimer()->start();

    /* enable EPOLLOUT event */
    Reactor::getInstance().getDemultoplexer().updateHandle(this->m_flowApp->getOutSockfd(), EPOLLOUT);
}
    
} // namespace faker_tsn
