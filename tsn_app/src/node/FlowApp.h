#ifndef _FLOW_APP_H
#define _FLOW_APP_H

#include <stdio.h>
#include <memory>
#include <algorithm>
#include <tinyxml2/tinyxml2.h>

#include "../utils/reflector/DynamicCreate.h"
#include "../utils/config/ConfigSetting.h"
#include "../utils/Log.h"
#include "../timer/ITimer.h"
#include "../timer/TimeContext.h"
#include "../core/port/IPort.h"
#include "../core/TSNContext.h"
#include "../core/InitTSNContextState.h"
#include "../networking/Reactor.h"
#include "../networking/interface/LinkLayerInterface.h"
#include "../networking/SendTSNFrameEventHandler.h"

using namespace tinyxml2;

namespace faker_tsn
{

class Flow {
public:
    long long uniqueID;
    long long pcp;
    long long size;
    long long vid;
    long long seq;
    Time::TimeInterval startOffset;
    Time::TimeInterval durationInterval;
    Time::TimeInterval periodInterval;
    std::string destMac;
    std::string groupMac;

    Flow() {
        this->uniqueID = 0;
        this->pcp = 7;
        this->vid = 1;
        this->seq = 0;
        this->size = 0;
        this->startOffset = Time::TimeInterval(0, 0);
        this->durationInterval = Time::TimeInterval(0, 0);
        this->periodInterval = Time::TimeInterval(0, 0);
        this->destMac = "00:00:00:00:00:00";
        this->groupMac = "";
    }

    std::string toString() {
        std::stringstream ss;
        ss << "uniqueID[" << this->uniqueID << "] ";
        ss << "pcp[" << this->pcp << "] ";
        ss << "vid[" << this->vid << "] ";
        ss << "size[" << this->size << "] ";
        ss << "destMac[" << this->destMac << "] ";
        ss << "groupMac[" << this->groupMac << "] ";
        ss << "startOffset[" << this->startOffset.toString() << "] ";
        ss << "durationInterval[" << this->durationInterval.toString() << "] ";
        ss << "periodInterval[" << this->periodInterval.toString() << "] ";
        return ss.str();
    }
};

class FlowApp : public IPort, public std::enable_shared_from_this<FlowApp>, public REFLECT_OBJECT, public DynamicCreator<FlowApp, const char*> {
private:
    std::string m_deviceName;   /* device name */
    std::shared_ptr<IPortState> m_state;
    int m_iSockfd;
    int m_oSockfd;
    std::shared_ptr<LinkLayerInterface> m_inf;

public:
    std::vector<Flow*> flows;
    int flowNum;
    int flowIndex;            /* index of current flow */
    int frameCount;
    int lastFrameSize;

public:
    FlowApp(const char*);

    ~FlowApp();

    /* load flow profiles */
    void loadScheduleXML(std::string);

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

    virtual void setTimer() override;

    friend void registerFlowToTimer(FlowApp* app, Flow* flow, Time::TimeInterval& interval);

    virtual std::string toString() {
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
        return 0;
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

#endif // _FLOW_APP_H