#include "StartTicker.h"

#include "../TSNContext.h"
#include "../RunTSNContextState.h"
#include "../../utils/monitor/MonitorTicker.h"

namespace faker_tsn
{

StartTicker::StartTicker(Time::TimePoint& start, Time::TimeInterval& expire, Time::TimeInterval& period) : Ticker(start, expire, period) {
    INFO("Start Ticker: " + this->toString());
}

StartTicker::StartTicker(Time::TimePoint& start, Time::TimeInterval& expire) : Ticker(start, expire) {
    INFO("Start Ticker: " + this->toString());
}

void StartTicker::operator()() {
    INFO("StartTicker on call");

    /* change tsn context state to run */
    INFO("TSN_CONTEXT_STATE: init -> run");
    std::shared_ptr<ITSNContextState> runState = std::make_shared<RunTSNContextState>();
    runState->doAction(TSNContext::getInstance());

    /* register gate control list item into timer */
    INFO("register timer");
    auto ports = TSNContext::getInstance().getPortManager()->getPorts();
    for (auto port: ports) {
        port->setTimer();
        Reactor::getInstance().getDemultoplexer().updateHandle(port->getOutSockfd(), EPOLLOUT);
    }

    /* register monitor ticker */
    Time::TimePoint start(0, 0);
    Time::TimeInterval expire(1, 0);
    Ticker* ticker = new MonitorTicker(start, expire);
    TimeContext::getInstance().getTimer()->addTicker(ticker);

    /* start timer */
    TimeContext::getInstance().getTimer()->start();
}
    
} // namespace faker_tsn
