#ifndef _TIMER_H
#define _TIMER_H

#include <signal.h>
#include <time.h>

#include <cstdint>
#include <memory>
#include <unordered_map>
#include <queue>
#include <vector>

#include "IClock.h"
#include "ITimer.h"
#include "../utils/Log.h"

namespace faker_tsn {

class PQTimer : public ITimer {
   private:
    std::priority_queue<
        Ticker*,
        std::vector<Ticker*>,
        std::greater<Ticker*>>
        m_tickers;                   /* ticker heap */
    std::unordered_map<
        long long, 
        Ticker*> 
        m_timers;                    /* <timer, ticker> */
    timer_t m_timerHandle;           /* timer handle */
    struct sigevent m_evp;           /* signal event struct */
    struct timespec m_spec;          /* time specification struct */
    struct itimerspec m_timeValue;   /* initial timer specification struct */
    struct sigaction m_sigAction;    /* signal action struct */
    std::shared_ptr<IClock> m_clock; /* clock */
    long long m_precision;           /* precision */

    friend void onAlarm(int signo);

   public:
    PQTimer();

    virtual ~PQTimer() override;

    /* enable timer */
    virtual void start() override;

    /* stop timer */
    virtual void stop() override;

    /* get precision */
    long long getPrecision() override;

    /* set timer */
    virtual void setTimer(Ticker* ticker);

    /* get ticker */
    virtual Ticker* getTicker() override;

    /* get ticker by id */
    virtual Ticker* getTicker(unsigned long id) override;

    /* get ticker by timerid */
    virtual Ticker* getTickerBy(long long id) override;

    /* add ticker */
    virtual void addTicker(Ticker* ticker) override;

    /* remove ticker */
    virtual void removeTicker() override;

    /* remove ticker by id*/
    virtual void removeTicker(unsigned long id) override;

    void showTickers() {
        INFO("No. of tickes = " + std::to_string(this->m_tickers.size()));
        while(!this->m_tickers.empty()) {
            INFO(this->m_tickers.top()->toString());
            this->m_tickers.pop();
        }
    }

    std::string toString() {}
};

}  // namespace faker_tsn

#endif  // _TIMER_H