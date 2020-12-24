#ifndef _FLOW_TICKER_H
#define _FLOW_TICKER_H

#include "../timer/ITimer.h"
#include "../timer/ITimer.h"
#include "../timer/IClock.h"
#include "../utils/reflector/Reflector.h"
#include "FlowApp.h"

namespace faker_tsn
{

class FlowTicker: public Ticker {
private:
    FlowApp *m_flowApp;
    bool m_isFlow;

public:
    FlowTicker(Time::TimePoint& start, Time::TimeInterval& expire, Time::TimeInterval& period, FlowApp* flowApp);

    FlowTicker(Time::TimePoint& start, Time::TimeInterval& expire, FlowApp* flowApp);

    ~FlowTicker();

    /* handler */
    void operator()() override;
};

    
} // namespace faker_tsn


#endif // _FLOW_TICKER_H