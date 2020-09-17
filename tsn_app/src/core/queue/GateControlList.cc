#include "GateControlList.h"

namespace faker_tsn {

// REFLECT(GateControlList);

void GateControlList::loadScheduleXML(std::string filename) {
    XMLDocument doc;
    doc.LoadFile(filename.c_str());
    XMLElement* schedule = doc.RootElement();
    /* get period */
    const char* timeUnit = schedule->Attribute("timeUnit");
    long interval = atoi(schedule->FirstChildElement("cycle")->GetText());
    INFO("interval:" + std::to_string(interval));
    Time::TimeInterval timeInterval = Time::converIntegerToTimeInterval(interval, timeUnit);
    /* get gate control list items */
    XMLElement* device = schedule->FirstChildElement("switch");
    while (!strcmp(device->Attribute("name"), ConfigSetting::getInstance().get<const char*>("deviceName"))) {
        device = device->NextSiblingElement();
    }
    XMLElement* port = device->FirstChildElement("port");
    while (static_cast<unsigned int>(atoi(port->Attribute("id"))) != this->m_portId) {
        port = port->NextSiblingElement();
    }
    XMLElement* entry = port->FirstChildElement("entry");
    while (entry) {
        long length = atoi(entry->FirstChildElement("length")->GetText());
        std::bitset<8> bitvector(std::string(entry->FirstChildElement("bitvector")->GetText()));
        INFO(std::to_string(length));
        INFO(bitvector.to_string());
        this->m_gcl.emplace_back(
            Time::converIntegerToTimeInterval(length, timeUnit),
            bitvector
        );
        entry = entry->NextSiblingElement();
    }
}

}  // namespace faker_tsn
