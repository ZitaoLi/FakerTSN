#ifndef _START_TICKER_H
#define _START_TICKER_H

#include "../../timer/ITimer.h"

namespace faker_tsn
{

class StartTicker: public Ticker {
public:
    StartTicker(Time::TimePoint& start, Time::TimeInterval& expire, Time::TimeInterval& period);

    StartTicker(Time::TimePoint& start, Time::TimeInterval& expire);

    virtual void operator()() override;
};
    
} // namespace faker_tsn

#endif // _START_TICKER_H