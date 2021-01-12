#include "StopTicker.h"

#include "../TSNContext.h"

namespace faker_tsn
{

StopTicker::StopTicker(Time::TimePoint& start, Time::TimeInterval& expire, Time::TimeInterval& period) : Ticker(start, expire, period) {
    INFO("Stop Ticker: " + this->toString());
}

StopTicker::StopTicker(Time::TimePoint& start, Time::TimeInterval& expire) : Ticker(start, expire) {
    INFO("Stop Ticker: " + this->toString());
}

void StopTicker::operator()() {
    // fllush
    std::fflush(stdout);

    exit(EXIT_SUCCESS);
}
    
} // namespace faker_tsn
