#include "ControlEventHandler.h"

#include "../timer/ITimer.h"
#include "../core/port/StartTicker.h"
#include "../timer/TimeContext.h"

namespace faker_tsn
{

ControlEventHandler::ControlEventHandler(HANDLE handle): m_handle(handle)
{

}

ControlEventHandler::~ControlEventHandler()
{

}

HANDLE ControlEventHandler::getHandle()
{
    return this->m_handle;
}

void ControlEventHandler::handle_event(EVENT_TYPE eventType)
{
    assert(eventType == EVENT_TYPE::READ);

    INFO("ControlEventHandler on call");
    
    /* receive command */
    char recvbuf[COMMAND_SIZE];
    int fd = this->m_handle;
    if (read(fd, recvbuf, COMMAND_SIZE) > 0) {
        INFO("Read success!");
    } else {
        INFO("Read error!");
    }

    /* parse command */
    /* fortmat: [cmd] [arg1] [arg2] */
    std::string command(recvbuf);
    INFO(">>> " + command);
    std::vector<std::string> keys(3);
    std::stringstream ss(command);
    std::string _t;
    for (int i = 0; i < 3; i++) {
        std::getline(ss, _t, ' ');
        keys[i] = _t;
    }
    if (keys[0] == "run") {
        // example: run 5 60
        INFO("set start time and runtime");
        INFO("start time: " + keys[1] + ", run time: " + keys[2]);
        std::string startTime = keys[1]; // seconds since the Epoch
        std::string runTime = keys[2]; // seconds
        Time::TimePoint start(std::stoi(keys[1]), 0);
        Time::TimeInterval interval(0, 0);
        Ticker* ticker = new StartTicker(start, interval);
        TimeContext::getInstance().getTimer()->addTicker(ticker);
        TimeContext::getInstance().getTimer()->start();
    } else if (keys[0] == "stop") {

    } else {
        // ignore
    }
}

} // namespace faker_tsn