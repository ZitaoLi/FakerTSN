#include "StopTicker.h"

#include "../TSNContext.h"
#include "../../utils/monitor/DataSpot.h"

namespace faker_tsn
{

extern DataSpot ds;

StopTicker::StopTicker(Time::TimePoint& start, Time::TimeInterval& expire, Time::TimeInterval& period) : Ticker(start, expire, period) {
    INFO("Stop Ticker: " + this->toString());
}

StopTicker::StopTicker(Time::TimePoint& start, Time::TimeInterval& expire) : Ticker(start, expire) {
    INFO("Stop Ticker: " + this->toString());
}

void StopTicker::operator()() {
    // ds.add<int>(1, {"switch1", "port0", "queue0", "frame_num"});
    // ds.add<int>(1, {"switch1", "port0", "queue0", "frame_num"});
    // ds.add<int>(2, {"switch1", "port1", "queue0", "frame_num"});
    // ds.add<int>(3, {"switch1", "port1", "queue1", "frame_num"});
    // ds.append<int>(1, {"switch1", "port0", "queue0", "buf_size"});
    // ds.append<int>(1, {"switch1", "port0", "queue0", "buf_size"});
    // ds.append<int>(1, {"switch1", "port0", "queue0", "buf_size"});

    std::string filename = ConfigSetting::getInstance().get<std::string>("data.path");
    ds.persist(filename);

    // fllush
    std::fflush(stdout);

    exit(EXIT_SUCCESS);
}
    
} // namespace faker_tsn
