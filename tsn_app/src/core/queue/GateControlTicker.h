#ifndef _GATE_CONTROL_TICKER_H
#define _GATE_CONTROL_TICKER_H

#include "../../timer/ITimer.h"
#include "../../utils/Log.h"
#include "GateControlList.h"

namespace faker_tsn
{

class GateControlTicker : public Ticker {
private:
    TimeAwareShaper* m_gcl;

public:
    GateControlTicker(Time::TimePoint& start, Time::TimeInterval& expire, Time::TimeInterval& period);

    GateControlTicker(Time::TimePoint& start, Time::TimeInterval& expire);

    GateControlTicker(Time::TimePoint& start, Time::TimeInterval& expire, Time::TimeInterval& period, TimeAwareShaper* gcl);

    GateControlTicker(Time::TimePoint& start, Time::TimeInterval& expire, TimeAwareShaper* gcl);

    /* handler */
    void operator()() override;
};
    
} // namespace faker_tsn

#endif // _GATE_CONTROL_TICKER_H