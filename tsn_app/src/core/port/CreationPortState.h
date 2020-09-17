#ifndef _CREATION_PORT_STATE
#define _CREATION_PORT_STATE

#include "IPortState.h"
#include "UpPortState.h"
#include "DownPortState.h"
#include "IPort.h"
#include "PortManager.h"
#include <sstream> 
#include <iomanip>
#include <pcap/pcap.h>

namespace faker_tsn
{

class CreationPortState: public IPortState, public std::enable_shared_from_this<CreationPortState>
{
private:
    /* data */
public:
    CreationPortState(/* args */);
    ~CreationPortState();

    /* template method */
    void doAction(std::shared_ptr<IPort>) override;

    /* create raw socket */
    int createSocket();

    /* open pcap */
    int openPCAP(const char*, std::shared_ptr<pcap_t>);
};
    
} // namespace faker_tsn


#endif // _CREATION_PORT_STATE