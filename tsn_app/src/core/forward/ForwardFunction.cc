#include "ForwardFunction.h"

#include "../../networking/Reactor.h"
#include "../TSNContext.h"
#include "../port/IPort.h"
#include "MacTable.h"

namespace faker_tsn {

void ForwardFunction::forward(const unsigned char* srcMac, void* data, size_t len, RELAY_ENTITY type) {
    INFO("Forward on call");

    /* get outbound ports */
    std::vector<unsigned short> ports = MacTable::findOutboundPorts(srcMac);

    // TODO unicast & multicast
    ports = std::vector<unsigned short>();
    ports.push_back(2);
    ports.push_back(3);

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
