#include "ForwardFunction.h"

#include "../../networking/Reactor.h"
#include "../TSNContext.h"
#include "../port/IPort.h"
#include "MacTable.h"
#include "MulticastMacTable.h"

namespace faker_tsn {

const char ForwardFunction::s_multicastPrefix = 0x01;

void ForwardFunction::forward(const unsigned char* srcMac, const unsigned char* destMac, void* data, size_t len, RELAY_ENTITY type) {
    INFO("Forward on call");

    std::vector<unsigned short> ports = std::vector<unsigned short>();

    /* distinguish unicast or multicast */
    if (memcmp(destMac, &ForwardFunction::s_multicastPrefix, 2) == 0) {
        // multicast
        INFO("multicast...");
        ports = MulticastMacTable::findOutboundPorts(destMac);
    } else {
        // unicast
        INFO("unicast...");
        ports = MacTable::findOutboundPorts(srcMac);
    };

    /* forward frame */
    auto portManager = TSNContext::getInstance().getPortManager();
    Selector& demultoplexer = Reactor::getInstance().getDemultoplexer();
    for (unsigned short index : ports) {
        auto port = portManager->getPort(index);
        INFO("Forward to " + port->toString());
        // send data into target port
        port->input(data, len, type);
        // enable EPOLLOUT event for target port
        demultoplexer.updateHandle(port->getOutSockfd(), EPOLLOUT);
    }
}

}  // namespace faker_tsn
