#ifndef _MULTICAST_MAC_TABLE_H
#define _MULTICAST_MAC_TABLE_H

#include <tinyxml2/tinyxml2.h>

#include <sstream>
#include <unordered_map>
#include <vector>

#include "../../networking/interface/MacAddress.h"
#include "../../utils/config/ConfigSetting.h"
#include "../port/IPort.h"
#include "MacTable.h"

using namespace tinyxml2;

namespace faker_tsn {

class MulticastMacTable {
    // vlan id, dest multicast mac address, ports
   public:
    /* forward item */
    struct ForwardItem {
        unsigned short vlanID;              /* vlan ID */
        std::vector<PORT_INDEX_TYPE> ports; /* outbound ports */
        long long timeout;                  /* timeout */

        ForwardItem(unsigned short vlanID, std::vector<unsigned short>& ports, long long timeout = -1) : vlanID(vlanID), ports(ports), timeout(timeout) {}

        ForwardItem(std::vector<unsigned short>& ports, long long timeout = -1) : vlanID(1), ports(ports), timeout(timeout) {}

        std::string toString() {
            std::stringstream ss;
            ss << "VID: " << vlanID << " ";
            ss << "PORT: ";
            for (auto it = ports.begin(); it != ports.end(); it++) {
                ss << *it << " ";
            }
            ss << "TIMEOUT: " << timeout;
            return ss.str();
        }
    };

    static std::unordered_map<mac_token, struct MulticastMacTable::ForwardItem>
        items; /* <mac addr, forward item> pair*/

    /* print multicast mac table */
    static std::string toString();

    /* load multicast mac table from .xml file */
    static void loadRouteXML(std::string filename);

    /* add a item into multicast mac table */
    static void addItem(const unsigned char* mac, std::vector<PORT_INDEX_TYPE>& ports, unsigned short vlanID, long long timeout);

    /* remove items by multicast mac address */
    static void removeItem(const unsigned char* mac);

    /* update items by multicast mac address */
    static void updateItem(const unsigned char* mac);

    /* find outbound ports by multicast mac address */
    static std::vector<PORT_INDEX_TYPE> findOutboundPorts(const unsigned char* mac);
};

}  // namespace faker_tsn

#endif  // _MULTICAST_MAC_TABLE_H