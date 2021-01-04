#ifndef _PORT_MANAGER_H
#define _PORT_MANAGER_H

#include <pcap/pcap.h>
#include "../../utils/Log.h"
#include "DataPort.h"
#include "CreationPortState.h"
#include "../../networking/Reactor.h"
#include <string>
#include <cstring>
#include <vector>
#include <memory>

namespace faker_tsn
{

class PortManager
{
private:
    std::vector<std::string> m_deviceNames;
    std::vector<std::shared_ptr<IPort>> m_ports;
    std::shared_ptr<IPort> m_console_port;
    
public:
    static const char *s_portFilterList[];
    static const int s_portFilterLen;

    PortManager();

    ~PortManager() = default;

    void init();

    /* get device name list */
    std::vector<std::string>& getAllDeviceName();

    /* get port using index */
    std::shared_ptr<IPort> getPort(uint8_t index);

    /* get ports */
    std::vector<std::shared_ptr<IPort>> getPorts();

    /* add port */
    void appendPort(std::shared_ptr<IPort>& port);

    /* find all device name */
    void findAllDeviceName();

    /* add device name */
    void appendDeviceName(const char*);

    /* create port from device name list */
    void createPortFromDeviceNameList();

    /* add IEventHandler to Reactor */
    void bindEventHandler(std::shared_ptr<Reactor> reactor);

    /* find mac address */
    static int findMacAddress(std::shared_ptr<IPort>, const char*, char[]);
};
    
} // namespace faker_tsn


#endif // _PORT_MANAGER_H