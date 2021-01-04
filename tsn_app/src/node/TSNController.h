#ifndef _TSN_CONTROLLER_H
#define _TSN_CONTROLLER_H

#include <memory>

#include "../core/port/IPort.h"
#include "../utils/reflector/DynamicCreate.h"
#include "../networking/interface/LinkLayerInterface.h"
#include "../networking/Reactor.h"
#include "../networking/CommandEventHandler.h"

namespace faker_tsn
{

class TSNController : 
    public IPort, 
    public std::enable_shared_from_this<TSNController>, 
    public REFLECT_OBJECT, 
    public DynamicCreator<TSNController, const char*> {
private:
    std::string m_deviceName;
    std::shared_ptr<IPortState> m_state;
    int m_csockfd;
    int m_iSockfd;
    int m_oSockfd;
    std::shared_ptr<LinkLayerInterface> m_inf;

private:
    std::string m_command;

public:
    TSNController(const char*);

    ~TSNController();

    /* change current state */
    virtual void setState(const std::shared_ptr<IPortState>&);

    /* return current state */
    virtual std::shared_ptr<IPortState>& getState();

    /* create raw socket */
    virtual void createSocket() override;

    /* register handler */
    virtual void registerEventHandler();

    /* input something into port */
    virtual void input(void*, size_t, RELAY_ENTITY type);

    /* output something */
    virtual void* output() override;

    virtual void setTimer() override {
        return;
    }

    virtual std::string toString() override {
        std::stringstream ss;
        ss << "[" << this->m_deviceName << "]";
        return ss.str();
    }

public:
    /* get device id */
    virtual uint8_t getDeviceID() const override {
        return 0;
    }

    /* Deprecated : set socket file descripter */
    virtual void setSockfd(const int) override {}

    /* set socket for sending */
    virtual void setInSockfd(const int) override {}

    /* set socket for receiving */
    virtual void setOutSockfd(const int) override {}

    /* get socket for sendingg */
    virtual int getInSockfd() override {
        return this->m_iSockfd;
    }

    /* get socket for receiving */
    virtual int getOutSockfd() override {
        return this->m_oSockfd;
    }

    /* get socket file descripter */
    virtual int getSockfd() override {
        return this->m_csockfd;
    }

    /* set device name */
    virtual void setDeviceName(const char*) override {}

    /* set mac address */
    virtual void setMacAddress(const char[]) override {}

    /* get device name */
    virtual const char* getDeviceName() const override {
        return this->m_deviceName.c_str();
    }

    /* test send data */
    virtual void sendTest() override {}

    /* set events types */
    virtual void setEventTypes(int) override {}

    /* get event types */
    virtual int getEventTypes() override {
        return 0;
    }
};
    
} // namespace faker_tsn


#endif // _TSN_CONTROLLER_H