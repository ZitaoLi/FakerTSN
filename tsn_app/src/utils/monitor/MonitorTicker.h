#ifndef _MONITOR_TICKER_H
#define _MONITOR_TICKER_H

#include <string>

#include "../../timer/ITimer.h"

namespace faker_tsn
{

class MonitorTicker : public Ticker {
private:
    std::string m_filename;

public:
    MonitorTicker(Time::TimePoint& start, Time::TimeInterval& expire);

    ~MonitorTicker();

    virtual void operator()() override;
};
    
} // namespace faker_tsn


#endif // _MONITOR_TICKER_H