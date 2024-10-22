#ifndef _DATA_PORT_H
#define _DATA_PORT_H

#include <exception>
#include <memory>
#include <sstream>

#include "../../networking/RecvTSNFrameEventHandler.h"
#include "../../networking/SendTSNFrameEventHandler.h"
#include "../../networking/frame/EnhancementTSNFrameBody.h"
#include "../../networking/frame/TSNFrameBody.h"
#include "../../networking/interface/LinkLayerInterface.h"
#include "../queue/QueueContext.h"
#include "../queue/IEEE8021Queue.h"
#include "../queue/IEEE8021QbvQueue.h"
#include "BaseRecoveryFunction.h"
#include "IPort.h"
#include "PortManager.h"

namespace faker_tsn {

class DataPort : public IPort, public std::enable_shared_from_this<DataPort>, public REFLECT_OBJECT, public DynamicCreator<DataPort, const char*> {
   private:
    const uint8_t m_deviceID;
    const char* m_deviceName;                        /* device name */
    char m_hwaddr[ETH_ALEN];                         /* hardware (MAC) address */
    int m_sockfd;                                    /* raw socket */
    int m_inSockfd;                                  /* socket for receiving */
    int m_outSockfd;                                 /* socket for sending */
    std::shared_ptr<QueueContext> m_queueContext;    /* queue context */
    std::shared_ptr<TimeAwareShaper> m_gcl;          /* gate control list */
    std::shared_ptr<LinkLayerInterface> m_interface; /* link layer interface */
    std::shared_ptr<IPortState> state;               /* port state */
    int m_eventTypes = 0;                            /* event type */
    bool m_isEnhanced;                               /* enhanced */
    BaseRecoveryFunction m_baseRecoveryFunction;     /* frame elimination */
    bool m_closedOutEvent;                           /* indicate whether to disable EPOLLOUT event or  */

   public:
    /* get gate control list */
    std::shared_ptr<TimeAwareShaper> getGCL() {
        return this->m_gcl;
    }

    /* get queue context */
    std::shared_ptr<QueueContext> getQueueContext() {
        return this->m_queueContext;
    }

    bool getClosedOutEvent() {
        return this->m_closedOutEvent;
    }

    void setClosedOutEvent(bool closed) {
        this->m_closedOutEvent = closed;
    }

   public:
    /* no. of port */
    static uint8_t s_portNum;

    DataPort(const char*);
    
    virtual ~DataPort() override;

    /* change current state */
    virtual void setState(const std::shared_ptr<IPortState>&) override;

    /* return current state */
    virtual std::shared_ptr<IPortState>& getState() override;

    virtual uint8_t getDeviceID() const override;

    virtual void setSockfd(const int) override;

    virtual int getSockfd() override;

    /* set socket for sending */
    virtual void setInSockfd(const int sockfd) override {
        this->m_inSockfd = sockfd;
    }

    /* set socket for receiving */
    virtual void setOutSockfd(const int sockfd) override {
        this->m_outSockfd = sockfd;
    }

    /* get socket for sendingg */
    virtual int getInSockfd() override {
        return this->m_inSockfd;
    }

    /* get socket for receiving */
    virtual int getOutSockfd() override {
        return this->m_outSockfd;
    }

    virtual void setMacAddress(const char[]) override;

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

    virtual void setTimer() override;

    virtual std::string toString() override {
        std::stringstream ss;
        ss << "Port[";
        ss << "id:" << std::to_string(this->m_deviceID);
        ss << "|";
        ss << "name:" << this->m_deviceName;
        ss << "]";
        return ss.str();
    }
};

}  // namespace faker_tsn

#endif  // _DATA_PORT_H