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
    IFrameBody* frame = reinterpret_cast<IFrameBody*>(data);
    for (unsigned short index : ports) {
        auto port = TSNContext::getInstance().getPortManager()->getPort(index);
        INFO("Forward to " + port->toString());
        // dup data
        IFrameBody* dupFrame = frame->copy();
        // send data into target port
        port->input(reinterpret_cast<IFrameBody*>(dupFrame), len, type);
        // enable EPOLLOUT event for target port
        Reactor::getInstance().getDemultoplexer().updateHandle(port->getOutSockfd(), EPOLLOUT);
    }
    delete(frame);
}

}  // namespace faker_tsn
