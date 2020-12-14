#include "PQTimer.h"

#include <assert.h>
#include <memory.h>
#include <time.h>

#include "../utils/Log.h"
#include "../utils/reflector/Reflector.h"
#include "../utils/config/ConfigSetting.h"
#include "TimeContext.h"
#include "IClock.h"

namespace faker_tsn {

REFLECT(PQTimer);

void onAlarm(int sig, siginfo_t* si, void* ucontext) {
    assert(sig == SIGRTMAX);
    INFO("ON ALARM");
    IClock* clock = Reflector::getNewInstance<IClock>("RealTimeClock");
    INFO(clock->now().toString());

    // timer_t* tidp = si->si_value.sival_ptr;
    printf("    sival_ptr = %p\n", si->si_value.sival_ptr);
    // printf("    sival_ptr = 0x%lx\n", (long long)tidp);

    // int _or = timer_getoverrun(&tidp);
    // if (_or == -1)
    //     printf("    timer_getoverrun\n");
    // else
    //     printf("    overrun count = %d\n", _or);

    /* get timer */
    ITimer* timer = TimeContext::getInstance().getTimer();

    Ticker* ticker = (Ticker*)si->si_value.sival_ptr;

    /* get first ticker */
    // std::shared_ptr<Ticker> ticker = timer->getTicker();
    INFO(ticker->toString());
    /* call handler */
    (*ticker)();
    
    delete ticker;

    // ucontext_t* context = (ucontext_t*)ucontext;
    // std::cout << "Timer Address = " << si->_sifields._timer.si_sigval.sival_ptr << std::endl;
    // INFO("sival_int = " + std::to_string(si->_sifields._timer.si_sigval.sival_int));
    // INFO("sival_int = " + std::to_string(si->si_value.sival_int));
    // INFO("sival_prt = " + std::to_string(*((int*)si->si_value.sival_ptr)));
    // INFO("sig_int = " + std::to_string(si->si_int));
    // INFO("sig_prt = " + std::to_string(*((int*)si->si_ptr)));
    // INFO("si_timerid = " + std::to_string(si->si_timerid));
    // INFO("si_overrun = " + std::to_string(si->si_overrun));
    // TODO
}

PQTimer::PQTimer() {
    /* set clock */
    this->m_clock.reset(Reflector::getNewInstance<IClock>("RealTimeClock"));

    /* set precision */
    this->m_precision = ConfigSetting::getInstance().get<int>("timePrecision");

    // /* install handler for timer signal */
    // memset(&this->m_sigAction, 0, sizeof(this->m_sigAction));
    // this->m_sigAction.sa_flags = SA_SIGINFO;  // unreliable signal ?
    // // this->m_sigAction.sa_handler = onAlarm;   // callback function
    // this->m_sigAction.sa_sigaction = onAlarm;  //signal-catching function with three arguments
    // sigemptyset(&this->m_sigAction.sa_mask);
    // if (sigaction(SIGRTMAX, &this->m_sigAction, NULL) == -1) {
    //     ERROR("invoke sigaction failed");
    //     // TODO handle error
    //     exit(EXIT_FAILURE);
    // }

    // /* create timer */
    // memset(&this->m_evp, 0, sizeof(this->m_evp));
    // this->m_evp.sigev_value.sival_ptr = &this->m_timerHandle;  // timer handle
    // this->m_evp.sigev_notify = SIGEV_SIGNAL;                   // signal trigger
    // this->m_evp.sigev_signo = SIGRTMAX;                        // max real time signal
    // // clock source CLOCK_REALTIME is settable and can be affected by system time changing caused by NTP or PTP
    // if (timer_create(CLOCK_REALTIME, &this->m_evp, &this->m_timerHandle) == -1) {
    //     ERROR("create posix timer failed");
    //     // TODO handle error
    //     exit(EXIT_FAILURE);
    // }
    // printf("timer ID Address is %p; ", &this->m_timerHandle);
    // printf("timer ID is 0x%lx\n", (long)this->m_timerHandle);
}

PQTimer::~PQTimer() {
    timer_delete(this->m_timerHandle);
}

long long PQTimer::getPrecision() {
    return this->m_precision;
}

void PQTimer::setTimer(Ticker* ticker) {
    /* create handler for timer signal */
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_flags = SA_SIGINFO;  // unreliable signal ?
    // sa.sa_handler = onAlarm;   // callback function
    sa.sa_sigaction = onAlarm;  //signal-catching function with three arguments
    sigemptyset(&sa.sa_mask);
    if (sigaction(SIGRTMAX, &sa, NULL) == -1) {
        ERROR("invoke sigaction failed");
        // TODO handle error
        exit(EXIT_FAILURE);
    }

    /* create timer */
    struct sigevent evp;
    timer_t timerid;
    memset(&evp, 0, sizeof(evp));
    evp.sigev_value.sival_ptr = ticker;    // timer handle
    evp.sigev_notify = SIGEV_SIGNAL;        // signal   
    evp.sigev_signo = SIGRTMAX;             // max real time signal
    // clock source CLOCK_REALTIME is settable and can be affected by system time changing caused by NTP or PTP
    if (timer_create(CLOCK_REALTIME, &evp, &timerid) == -1) {
        ERROR("create posix timer failed");
        // TODO handle error
        exit(EXIT_FAILURE);
    }
    printf("    sival_ptr = %p\n", evp.sigev_value.sival_ptr);
    printf("    timer ID Address is %p\n", &timerid);
    printf("    timer ID is 0x%lx\n", (size_t)timerid);

    // /* associate timer with ticker */
    // this->m_timers.insert({(size_t)timerid, ticker});

    /* set timer */
    if (ticker->getStart() != 0) {
        // TODO
        ERROR("absolute start time is not supported");
        exit(EXIT_FAILURE);
    } else {
        Time::TimePoint now = Reflector::getNewInstance<IClock>("RealTimeClock")->now();
        ticker->setLaunchTime(now);
        Time::TimePoint launchTime = ticker->getLanunchTime();
        Time::TimePoint tp = launchTime + ticker->getExpire();
        INFO("Set Sec = " + std::to_string(tp.sec) + ", Nsec = " + std::to_string(tp.nsec));

        struct itimerspec its;
        memset(&its, 0, sizeof(its));
        its.it_value.tv_sec = tp.sec;
        its.it_value.tv_nsec = tp.nsec;
        if (timer_settime(timerid, TIMER_ABSTIME, &its, NULL) == -1) {
            ERROR("set posix timer failed");
            exit(EXIT_FAILURE);
            // TODO handle error
        }
    }

    // timer_t timer;
    // struct sigevent evp;
    // memset(&evp, 0, sizeof(this->m_evp));
    // evp.sigev_value.sival_ptr = &timer;  // timer handle
    // // evp.sigev_value.sival_int = 0x00000001;  // set ticker id to sival_int
    // evp.sigev_notify = SIGEV_SIGNAL;  // signal trigger
    // evp.sigev_signo = SIGRTMAX;       // max real time signal
    // // clock source CLOCK_REALTIME is settable and can be affected by system time changing caused by NTP or PTP
    // if (timer_create(CLOCK_REALTIME, &evp, &timer) == -1) {
    //     ERROR("create posix timer failed");
    //     // TODO handle error
    //     exit(EXIT_FAILURE);
    // }
    // printf("timer ID is 0x%lx\n", (long)timer);
    // // TIMER_ABSTIME indicates absolute time
    // if (timer_settime(timer, TIMER_ABSTIME, &its, NULL) == -1) {
    //     ERROR("set posix timer failed");
    //     // TODO handle error
    // }
    // INFO("Create Timer with ID = " + std::to_string(*((int*)timer)));
    // INFO("SET TIMER");
}

void PQTimer::start() {
LOOP:
    /* get first ticker */
    Ticker* ticker = this->getTicker();
    INFO("Get Ticker : " + ticker->toString());
    // this->removeTicker();

    /* set timer */
    this->setTimer(ticker);

    /* remove ticker */
    this->removeTicker();

    Time::TimePoint start1 = ticker->getStart();
    Time::TimePoint end1 = start1 + ticker->getExpire();

    /* reset ticker */
    if (ticker->isPeriodic()) {
        ticker->setStart(this->m_clock->now() + ticker->getPeriod());
        this->addTicker(ticker);
    }

    /* check next ticker */
    ticker = this->getTicker();
    if (ticker) {
        Time::TimePoint start2 = ticker->getStart();
        if (start2 >= start1 && start2 < end1) {
            goto LOOP;
        }
    }
}

void PQTimer::stop() {
    // TODO
}

Ticker* PQTimer::getTicker() {
    if (this->m_tickers.size() == 0) return nullptr;
    return this->m_tickers.top();
}

Ticker* PQTimer::getTicker(unsigned long id) {
    // TODO
    return nullptr;
}

Ticker* PQTimer::getTickerBy(long long id) {
    if (this->m_timers.find(id) != this->m_timers.end())
        return this->m_timers[id];
    return nullptr;
}

void PQTimer::addTicker(Ticker* ticker) {
    this->m_tickers.push(ticker);
}

void PQTimer::removeTicker() {
    this->m_tickers.pop();
}

void PQTimer::removeTicker(unsigned long id) {
    // TODO
}

}  // namespace faker_tsn
