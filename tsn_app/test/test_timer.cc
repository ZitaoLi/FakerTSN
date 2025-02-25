#include <faker_tsn.h>
#include <gtest/gtest.h>
#include <memory.h>
#include <time.h>
// #include <linux/time.h>

#include <chrono>
#include <iostream>
#include <thread>

#include "../src/timer/ITimer.h"

using namespace std;
using namespace faker_tsn;

static void TestClockGetTime() {
    struct timespec ts;
    memset(&ts, 0, sizeof(ts));

    clock_gettime(CLOCK_REALTIME, &ts);
    cout << "[CLOCK_REALTIME] ";
    cout << "sec:" << ts.tv_sec << " nsec:" << ts.tv_nsec << endl;

    clock_gettime(CLOCK_MONOTONIC, &ts);
    cout << "[CLOCK_MONOTONIC] ";
    cout << "sec:" << ts.tv_sec << " nsec:" << ts.tv_nsec << endl;
}

static void TestTimeClass() {
    Time::TimePoint timePoint;
    timePoint.setNow();
    INFO(timePoint.toString());

    Time::TimeInterval timeInterval(1, 0);
    timePoint = timePoint + timeInterval;
    INFO(timePoint.toString());

    // sleep 3s
    chrono::nanoseconds sleepTime(2000000000);
    this_thread::sleep_for(sleepTime);  // millisecond deviation
    cout << "sleep 2s" << endl;

    Time::TimePoint _timePoint;
    _timePoint.setNow();
    INFO(_timePoint.toString());
    Time::TimeInterval timeInterval1 = _timePoint - timePoint;
    INFO(timeInterval1.toString());
}

int sec = 0;
int nsec = 120000;

Time::TimePoint g_timePoint1;
int g_flag1 = 1;
static void HandlerSignal(int signo) {
    INFO("ON SIGNAL");
    Time::TimePoint timePoint;
    timePoint.setNow();
    Time::TimeInterval interval = timePoint - g_timePoint1;
    INFO(interval.toString());
    g_flag1 = 0;
}

Time::TimePoint g_timePoint2;
int g_flag2 = 1;
static void HandlerThread(union sigval) {
    INFO("ON THREAD");
    Time::TimePoint timePoint;
    timePoint.setNow();
    Time::TimeInterval interval = timePoint - g_timePoint2;
    INFO(interval.toString());
    g_flag2 = 0;
}

static void TestTimerSetTimeSignal() {
    /* create timer */
    timer_t timer;
    struct sigevent evp;
    memset(&evp, 0, sizeof(evp));
    /* use signal */
    evp.sigev_value.sival_ptr = &timer;
    evp.sigev_notify = SIGEV_SIGNAL;
    evp.sigev_signo = SIGRTMAX - 1;
    signal(SIGRTMAX - 1, HandlerSignal);
    if (timer_create(CLOCK_REALTIME, &evp, &timer) == -1) {
        ERROR("create timer failed");
    }

    /* set timer */
    Time::TimePoint timePoint;
    timePoint.setNow();
    g_timePoint1 = timePoint;
    Time::TimeInterval interval(sec, nsec);
    timePoint = timePoint + interval;

    struct itimerspec its;
    memset(&its, 0, sizeof(its));
    its.it_value.tv_sec = timePoint.sec;
    its.it_value.tv_nsec = timePoint.nsec;

    if (timer_settime(timer, TIMER_ABSTIME, &its, NULL) == -1) {
        ERROR("set timer failed");
    }

    while (g_flag1) {
        // INFO("...");
    }
}

static void TestTimerSetTimeThread() {
    /* create timer */
    timer_t timer;
    struct sigevent evp;
    memset(&evp, 0, sizeof(evp));
    /* use thread */
    evp.sigev_value.sival_ptr = &timer;
    evp.sigev_notify = SIGEV_THREAD;
    evp.sigev_notify_function = HandlerThread;
    evp.sigev_notify_attributes = NULL;
    if (timer_create(CLOCK_REALTIME, &evp, &timer) == -1) {
        ERROR("create timer failed");
    }

    /* set timer */
    Time::TimePoint timePoint;
    timePoint.setNow();
    g_timePoint2 = timePoint;
    Time::TimeInterval interval(sec, nsec);
    timePoint = timePoint + interval;

    struct itimerspec its;
    memset(&its, 0, sizeof(its));
    its.it_value.tv_sec = timePoint.sec;
    its.it_value.tv_nsec = timePoint.nsec;

    if (timer_settime(timer, TIMER_ABSTIME, &its, NULL) == -1) {
        ERROR("set timer failed");
    }

    while (g_flag2) {
        // INFO("...");
    }
}

static void TestPQTimer() {
    PQTimer timer;
    IClock* clock = Reflector::getNewInstance<IClock>("RealTimeClock");
    INFO(clock->now().toString());

    Time::TimePoint start = clock->now();
    Time::TimeInterval expire = Time::TimeInterval(4, 0);  // +4s
    Time::TimeInterval period(0, 0);
    INFO("Start Time = " + start.toString());
    INFO("Expire Time = " + expire.toString());
    INFO("Period Time = " + period.toString());
    Ticker* ticker1 = new Ticker(start, expire, period);
    INFO("Ticker ID: " + std::to_string(ticker1->getId()));
    INFO("is periodic: " + std::to_string(ticker1->isPeriodic()));

    // start += Time::TimeInterval(2, 0);
    Time::TimeInterval expire2 = Time::TimeInterval(2, 0);
    INFO("Start Time = " + start.toString());
    INFO("Expire Time = " + expire.toString());
    INFO("Period Time = " + period.toString());
    Ticker* ticker2 = new Ticker(start, expire2, period);
    INFO("Ticker ID: " + std::to_string(ticker2->getId()));
    INFO("is periodic: " + std::to_string(ticker2->isPeriodic()));

    timer.addTicker(ticker1);
    timer.addTicker(ticker2);
    // timer.showTickers();
    timer.start();

    while (true) {
    }
}

static void TestTimeContext() {
    ITimer* timer = TimeContext::getInstance().getTimer();
    IClock* clock = Reflector::getNewInstance<IClock>("RealTimeClock");
    INFO(clock->now().toString());

    Time::TimePoint start = clock->now();
    Time::TimeInterval expire = Time::TimeInterval(4, 0);  // +4s
    Time::TimeInterval period(0, 0);
    INFO("Start Time = " + start.toString());
    INFO("Expire Time = " + expire.toString());
    INFO("Period Time = " + period.toString());
    Ticker* ticker1 = new Ticker(start, expire, period);
    INFO("Ticker ID: " + std::to_string(ticker1->getId()));
    INFO("is periodic: " + std::to_string(ticker1->isPeriodic()));

    timer->addTicker(ticker1);
    timer->start();

    while (true) {
    }
}

TEST(TEST_TIMER, TEST_CLOCK_GET_TIME) {
    // TestClockGetTime();
}

TEST(TEST_TIMER, TIME_CLASS) {
    // TestTimeClass();
}

TEST(TEST_TIMER, TEST_TIMER_SET_TIME) {
    /* real-time signal is better than thread */
    // TestTimerSetTimeSignal();
    // TestTimerSetTimeThread();
}

TEST(TEST_TIMER, TEST_PQTIMER) {
    // TestPQTimer();
}

TEST(TEST_TIMER, TEST_TIME_CONTEXT) {
    TestTimeContext();
}