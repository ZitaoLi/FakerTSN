#include "MulticastMacTable.h"

namespace faker_tsn {

std::unordered_map<mac_token, struct MulticastMacTable::ForwardItem>
    MulticastMacTable::items; /* <mac addr, forward item> pair*/

/* print multicast mac table */
std::string MulticastMacTable::toString() {
    std::stringstream ss;
    auto it = MulticastMacTable::items.begin();
    for (; it != MulticastMacTable::items.end(); it++) {
        ss << "MAC_TOKEN: " << it->first << " ";
        ss << it->second.toString();
    }
    return ss.str();
}

/* add a item into multicast mac table */
void MulticastMacTable::addItem(const unsigned char* mac, std::vector<PORT_INDEX_TYPE>& ports, unsigned short vlanID, long long timeout = -1) {
    mac_token temp;
    memcpy(&temp, mac, ETH_ALEN);
    ForwardItem forwardItem(1, ports, timeout);
    MulticastMacTable::items.emplace(std::move(temp), std::move(forwardItem));
}

/* remove items by multicast mac address */
void MulticastMacTable::removeItem(const unsigned char* mac) {
    mac_token temp;
    memcpy(&temp, mac, ETH_ALEN);
    auto it = MulticastMacTable::items.find(temp);
    if (it != MulticastMacTable::items.end()) {
        MulticastMacTable::items.erase(it);
    }
}

/* update items by multicast mac address */
void MulticastMacTable::updateItem(const unsigned char* mac) {}

/* find outbound ports by multicast mac address */
std::vector<PORT_INDEX_TYPE> MulticastMacTable::findOutboundPorts(const unsigned char* mac) {
    mac_token temp;
    memcpy(&temp, mac, ETH_ALEN);
    auto it = MulticastMacTable::items.find(temp);
    if (it != MulticastMacTable::items.end()) {
        return it->second.ports;
    } else {
        return std::vector<unsigned short>();
    }
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
            const char* vlanIDString = multicastAddress->Attribute("vlanID");
            std::vector<unsigned short> portIndexes;
            portIndexes = MacTable::parsePortIndex(portString);
            unsigned char mac[ETH_ALEN];
            MacTable::parseMacAddress(macString, mac);
            unsigned short vlanID = 1;
            if (vlanIDString != nullptr) {
                vlanID = static_cast<unsigned short>(atoi(vlanIDString));
            }
            MulticastMacTable::addItem(mac, portIndexes, vlanID);
        }
        multicastAddress = multicastAddress->NextSiblingElement();
    }
}

}  // namespace faker_tsn
