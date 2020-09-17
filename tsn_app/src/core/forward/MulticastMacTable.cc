#include "MulticastMacTable.h"

namespace faker_tsn {

std::unordered_map<mac_token, struct MulticastMacTable::ForwardItem>
    MulticastMacTable::items; /* <mac addr, forward item> pair*/

/* print multicast mac table */
std::string toString() {
    std::stringstream ss;
    auto it = MulticastMacTable::items.begin();
    for (; it != MulticastMacTable::items.end(); it++) {
        ss << "MAC_TOKEN: " << it->first << " ";
        ss << it->second.toString();
    }
    return ss.str();
}

/* load multicast mac table from .xml file */
void MulticastMacTable::loadRouteXML(std::string filename) {
    XMLDocument doc;
    doc.LoadFile(filename.c_str());
    XMLElement* root = doc.RootElement();
    /* get filetr database */
    XMLElement* fileterDB = root->FirstChildElement("filteringDatabase");
    while (fileterDB) {
        if (strcmp(fileterDB->Attribute("id"), ConfigSetting::getInstance().get<const char*>("deviceName")) == 0)
            break;
        fileterDB = fileterDB->NextSiblingElement();
    }
    if (!fileterDB) {
        ERROR("lack of filerdatabase");
        throw LackOfTagException("filteringDatabase");
    }
    /* get static list */
    XMLElement* staticList = fileterDB->FirstChildElement("static");
    /* get forward contents */
    XMLElement* forwardList = staticList->FirstChildElement("forward");
    /* get multicastAddress */
    XMLElement* multicastAddress = forwardList->FirstChildElement("multicastAddress");
    while (multicastAddress) {
        if (strcmp(multicastAddress->Name(), "multicastAddress") == 0) {
            const char* portString = multicastAddress->Attribute("ports");
            const char* macString = multicastAddress->Attribute("macAddress");
            std::vector<unsigned short> portIndexes;
            portIndexes = MacTable::parsePortIndex(portString);
            unsigned char mac[ETH_ALEN];
            MacTable::parseMacAddress(macString, mac);
            MacTable::addItem(mac, portIndexes);
        }
        multicastAddress = multicastAddress->NextSiblingElement();
    }
}

/* add a item into multicast mac table */
void MulticastMacTable::addItem(const unsigned char* mac, std::vector<PORT_INDEX_TYPE>& port, long long timeout = -1) {}

/* remove items by multicast mac address */
void MulticastMacTable::removeItem(const unsigned char* mac) {}

/* update items by multicast mac address */
void MulticastMacTable::updateItem(const unsigned char* mac) {}

/* find outbound ports by multicast mac address */
std::vector<PORT_INDEX_TYPE> MulticastMacTable::findOutboundPorts(const unsigned char* mac) {}

}  // namespace faker_tsn
