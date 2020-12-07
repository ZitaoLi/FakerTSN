#ifndef _GATE_CONTROL_TICKER_H
#define _GATE_CONTROL_TICKER_H

#include "../../timer/ITimer.h"
#include "../../utils/Log.h"

namespace faker_tsn
{

class GateControlTicker : public Ticker {

public:
    GateControlTicker(Time::TimePoint& start, Time::TimeInterval& expire, Time::TimeInterval& period) : Ticker(start, expire, period) {}

    GateControlTicker(Time::TimePoint& start, Time::TimeInterval& expire) : Ticker(start, expire) {}

    /* handler */
    void operator()() override {
        INFO("oncall GateControlTicker");
    }
};
    
} // namespace faker_tsn

#endif // _GATE_CONTROL_TICKER_H