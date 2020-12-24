#ifndef _CONSOLE_PORT_H
#define _CONSOLE_PORT_H

#include <sstream>

#include "../../networking/StdinEventHandler.h"
#include "IPort.h"

namespace faker_tsn {

class ConsolePort : public IPort, public std::enable_shared_from_this<ConsolePort> {
   private:
    int m_sockfd;
    std::shared_ptr<IPortState> state;
    int m_eventTypes = 0; /* event type */
   public:
    /* no. of port */
    static uint8_t s_portNum;

    ConsolePort();
    virtual ~ConsolePort() override;

    /* change current state */
    virtual void setState(const std::shared_ptr<IPortState>&) override;

    /* return current state */
    virtual std::shared_ptr<IPortState>& getState() override;

    virtual uint8_t getDeviceID() const override;

    virtual void setSockfd(const int) override;

    virtual int getSockfd() override;

    /* set socket for sending */
    virtual void setInSockfd(const int) override {}

    /* set socket for receiving */
    virtual void setOutSockfd(const int) override {}

    /* get socket for sendingg */
    virtual int getInSockfd() override { return 0; }

    /* get socket for receiving */
    virtual int getOutSockfd() override { return 0; }

    virtual void setMacAddress(const char[]);

    virtual void setDeviceName(const char*) override;

    virtual const char* getDeviceName() const override;

    virtual void sendTest() override;

    virtual void createSocket() override;

    virtual void setEventTypes(int) override;

    virtual int getEventTypes() override;

    virtual void registerEventHandler() override;

    /* input something into port */
    virtual void input(void*, size_t, RELAY_ENTITY type) override;

    /* output something */
    virtual void* output() override;

    virtual void setTimer() override {}

    virtual std::string toString() override {
        std::stringstream ss;
        ss << "console";
        return ss.str();
    }
};

}  // namespace faker_tsn

#endif  // _CONSOLE_PORT_H