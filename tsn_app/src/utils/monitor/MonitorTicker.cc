#include "MonitorTicker.h"
#include "../../timer/TimeContext.h"
#include "../config/ConfigSetting.h"
#include "DataSpot.h"

namespace faker_tsn
{

extern DataSpot ds;

MonitorTicker::MonitorTicker(Time::TimePoint& start, Time::TimeInterval& expire) : Ticker(start, expire) {
    this->m_filename = ConfigSetting::getInstance().get<std::string>("data.path");
}

MonitorTicker::~MonitorTicker() {}

void MonitorTicker::operator()() {
    INFO("MonitorTicker on call");

    /* persist data */
    ds.persist(this->m_filename);
    std::fflush(stdout);

    /* next ticker */
    Time::TimePoint start(0, 0);
    Time::TimeInterval expire = this->getExpire();
    Ticker* ticker = new MonitorTicker(start, expire);
    TimeContext::getInstance().getTimer()->addTicker(ticker);

    /* enable timer */
    TimeContext::getInstance().getTimer()->start();
}
    
} // namespace faker_tsn
