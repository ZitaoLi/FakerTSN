#include "GateControlList.h"
#include "GateControlTicker.h"
#include "TransmissionGate.h"

namespace faker_tsn {

// REFLECT(GateControlList);

// TODO
std::string GateControlList::toString() {
    std::stringstream ss;
    for (int i = 0; i < this->m_gateSize; i++) {
        if (this->m_gates[i]->isOpen()) {
            ss << "1";
        } else {
            ss << "0";
        }
    }
    return ss.str();
}

GateControlList::GateControlList(unsigned int portId) : m_portId(portId), m_length(0), m_cursor(0), m_gateSize(0) {
    /* load config file */
    INFO("[" + std::to_string(this->m_portId) + "] load gcl.xml");
    std::string filename = "./config/gcl.xml";
    this->loadScheduleXML(filename);

    /* register ticker to timer */
    INFO("[" + std::to_string(this->m_portId) + "] register ticker to timer");
    if (this->m_gcl.size() != 0) {
        Time::TimePoint startTime(0, 0);
        GateControlListItem item = this->m_gcl[this->m_cursor];
        INFO("[" + std::to_string(this->m_portId) + "] " + item.toString());
        Ticker* ticker = new GateControlTicker(
            startTime,
            item.m_timeInterval,
            this);
        TimeContext::getInstance().getTimer()->addTicker(ticker);
    }
}

void GateControlList::loadScheduleXML(std::string filename) {
    XMLDocument doc;
    doc.LoadFile(filename.c_str());
    XMLElement* schedule = doc.RootElement();
    /* get period */
    const char* timeUnit = schedule->Attribute("timeUnit");
    INFO("time unit: \t" + std::string(timeUnit));
    long interval = atoi(schedule->FirstChildElement("cycle")->GetText());
    INFO("hyper period:\t" + std::to_string(interval));
    Time::TimeInterval hyperPeriod = Time::converIntegerToTimeInterval(interval, timeUnit);
    this->m_period = hyperPeriod;
    /* get gate control list items */
    XMLElement* device = schedule->FirstChildElement("switch");
    INFO("load " + std::string(ConfigSetting::getInstance().get<const char*>("deviceName")));
    while (strcmp(device->Attribute("name"), ConfigSetting::getInstance().get<const char*>("deviceName")) != 0) {
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
        INFO("interval:\t" + std::to_string(length) + "\tstates:\t" + bitvector.to_string());
        this->m_gcl.emplace_back(
            Time::converIntegerToTimeInterval(length, timeUnit),
            bitvector
        );
        entry = entry->NextSiblingElement();
    }
}

/* get current item */
GateControlListItem GateControlList::getCurrentItem() {
    return this->m_gcl[this->m_cursor];
}

/* append gate */
void GateControlList::appendGate(std::shared_ptr<TransmissionGate> gate) {
    INFO("[" + std::to_string(m_portId) + "] append a gate");
    this->m_gates.push_back(std::move(gate));
    this->m_gateSize += 1;
}

/* update all gates */
void GateControlList::updateGates() {
    INFO("[" + std::to_string(this->m_portId) + "] update gates");
    INFO("[" + std::to_string(this->m_portId) + "] -> " + this->m_gcl[this->m_cursor].toString());
    auto item = this->m_gcl[this->m_cursor];
    for (int i = 0; i < this->m_gateSize; i++) {
        if (item.m_gateStates.test(i)) {
            this->m_gates[i]->onUpdate(i, true); // open gate
        } else {
            this->m_gates[i]->onUpdate(i, false); // close gate
        }
    }
    this->m_cursor++; // move to next item
}

}  // namespace faker_tsn
