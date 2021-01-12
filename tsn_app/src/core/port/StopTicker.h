#ifndef _STOP_TICKER_H
#define _STOP_TICKER_H

#include "../../timer/ITimer.h"

namespace faker_tsn
{

class StopTicker : public Ticker {
public:
    StopTicker(Time::TimePoint& start, Time::TimeInterval& expire, Time::TimeInterval& period);

    StopTicker(Time::TimePoint& start, Time::TimeInterval& expire);

    virtual void operator()() override;
};
    
} // namespace faker_tsn

#endif // _STOP_TICKER_H