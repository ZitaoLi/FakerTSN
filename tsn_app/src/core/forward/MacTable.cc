#include "MacTable.h"

namespace faker_tsn {

std::unordered_map<mac_token, ForwardPair> MacTable::items;
std::unordered_map<unsigned short, unsigned char*> MacTable::peers;

std::string MacTable::toString() {
    std::stringstream ss;
    for (auto it = MacTable::items.begin(); it != MacTable::items.end(); it++) {
        ss << "MAC_TOKEN: " << it->first;
        ss << " ";
        ss << it->second.toString();
        ss << "\n";
    }
    for (auto it = MacTable::peers.begin(); it != MacTable::peers.end(); it++) {
        char mac_str[32];
        unsigned char* dest_mac = it->second;
        sprintf(mac_str, "%.2X:%.2X:%.2X:%.2X:%.2X:%.2X", dest_mac[0], dest_mac[1], dest_mac[2], dest_mac[3], dest_mac[4], dest_mac[5]);
        ss << "PORT_ID: " << std::to_string(it->first);
        ss << " ";
        ss << "PEER_MAC: " << std::string(mac_str);
        ss << "\n";
    }
    return ss.str();
}

void MacTable::loadRouteXML(std::string filename) {
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
    /* get port list */
    XMLElement* portList = staticList->FirstChildElement("ports");
    /* get port */
    XMLElement* port = portList->FirstChildElement("port");
    while (port) {
        unsigned short portId = atoi(port->Attribute("id"));
        const char* macString = port->Attribute("peerMacAddress");
        unsigned char mac[ETH_ALEN];
        MacTable::parseMacAddress(macString, mac);
        MacTable::addPeer(portId, mac);
        port = port->NextSiblingElement();
    }
    /* get forward contents */
    XMLElement* forwardList = staticList->FirstChildElement("forward");
    /* get individual address */
    XMLElement* individualAddress = forwardList->FirstChildElement("individualAddress");
    while (individualAddress) {
        if (strcmp(individualAddress->Name(), "individualAddress") == 0) {
            const char* portString = individualAddress->Attribute("port");
            const char* macString = individualAddress->Attribute("macAddress");
            std::vector<unsigned short> portIndexes;
            portIndexes = MacTable::parsePortIndex(portString);
            unsigned char mac[ETH_ALEN];
            MacTable::parseMacAddress(macString, mac);
            MacTable::addItem(mac, portIndexes);
        }
        individualAddress = individualAddress->NextSiblingElement();
    }
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

void MacTable::parseMacAddress(const std::string& macString, unsigned char* mac) {
    for (int i = 0, j = 0; i < 6; i++, j += 3) {
        std::string temp = macString.substr(j, 2);
        int n = std::stoi(temp, 0, 16);
        memcpy(mac + i, (unsigned char*)&n, 1);
    }
}

/* conver port string to port vector */
std::vector<unsigned short> MacTable::parsePortIndex(const std::string& portString) {
    std::vector<unsigned short> output;
    if (portString.find(" ") == std::string::npos) {  // single port
        unsigned short portIndex = std::atoi(portString.c_str());
        output.push_back(portIndex);
    } else {  // multiple ports
        std::string strs = portString + " ";
        size_t start = 0;
        size_t end = strs.find(" ");
        while (end != std::string::npos) {
            unsigned short portIndex = std::atoi(strs.substr(start, end - start).c_str());
            output.push_back(portIndex);
            start = end + 1;
            end = strs.find(" ", start);
        }
    }
    return output;
}

}  // namespace faker_tsn
