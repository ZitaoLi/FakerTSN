#include "EnhancedGateControlList.h"
#include "GateControlTicker.h"
#include "TransmissionGate.h"

namespace faker_tsn
{

EnhancedGateControlList::EnhancedGateControlList(unsigned int portId) : 
    m_portId(portId), 
    m_length(0), 
    m_currentCursor(0), 
    m_nextCursor(0),
    m_gateSize(0) {
    /* load config file */
    INFO("[" + std::to_string(this->m_portId) + "] load gcl.xml");
    std::string profilePath = ConfigSetting::getInstance().get<std::string>("profile.path");
    std::string filename = profilePath + "/gcl.xml";
    this->loadScheduleXML(filename);
}

EnhancedGateControlList::~EnhancedGateControlList() {
}

std::string EnhancedGateControlList::toString() {
    std::stringstream ss;
    ss << "length[" << std::to_string(this->m_length) << "] ";
    ss << "period[" << this->m_period.toString() << "] ";
    ss << "cursor(current)[" << std::to_string(this->m_currentCursor) << "] ";
    ss << "cursor(next)[" << std::to_string(this->m_nextCursor) << "] ";
    return ss.str();
}

void EnhancedGateControlList::loadScheduleXML(std::string filename) {
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
        unsigned int uniqueId = atoi(entry->FirstChildElement("uniqueID")->GetText());
        unsigned int phase = atoi(entry->FirstChildElement("phase")->GetText());
        INFO("interval:" + std::to_string(length) + 
            "\tstates:" + bitvector.to_string() + 
            "\tuniqueId:" + std::to_string(uniqueId) + 
            "\tphase" + std::to_string(phase));
        EnhancedGateControlListItem* item = new EnhancedGateControlListItem(
            Time::converIntegerToTimeInterval(length, timeUnit), 
            bitvector,
            uniqueId,
            phase);
        this->m_gcl.emplace_back(item);
        INFO(item->toString());
        entry = entry->NextSiblingElement();
        this->m_length++;
    }
}

void EnhancedGateControlList::registerGCLfromSchedules() {
    /* register ticker to timer */
    INFO("[" + std::to_string(this->m_portId) + "] register ticker to timer");
    if (this->m_gcl.size() != 0) {
        Time::TimePoint startTime(0, 0);
        EnhancedGateControlListItem* item = this->m_gcl[this->m_nextCursor];
        INFO("[" + std::to_string(this->m_portId) + "] " + item->toString());
        Ticker* ticker = new GateControlTicker(
            startTime,
            item->m_timeInterval,
            this);
        TimeContext::getInstance().getTimer()->addTicker(ticker);
    }
}

GateControlListItem* EnhancedGateControlList::getItem(unsigned int index) {
    return this->m_gcl[index];
}

GateControlListItem* EnhancedGateControlList::getCurrentItem() {
    return this->m_gcl[this->m_currentCursor];
}

GateControlListItem* EnhancedGateControlList::getNextItem() {
    return this->m_gcl[this->m_nextCursor];
}

void EnhancedGateControlList::appendGate(std::shared_ptr<TransmissionGate> gate) {
    INFO("[" + std::to_string(m_portId) + "] append a gate of queue [" + std::to_string(gate->getPCP()) + "]");
    this->m_gates.push_back(std::move(gate));
    this->m_gateSize += 1;
}

void EnhancedGateControlList::updateGates() {
    INFO("[" + std::to_string(this->m_portId) + "] update gates");
    INFO("[" + std::to_string(this->m_portId) + "] -> " + this->m_gcl[this->m_nextCursor]->toString());
    auto item = this->m_gcl[this->m_nextCursor];
    for (int i = this->m_gateSize - 1; i >= 0 ; i--) {
        if (item->m_gateStates.test(i)) {
            INFO("[" + std::to_string(this->m_portId) + "] gate [" + std::to_string(this->m_gates[i]->getPCP()) + "] -> 1");
            this->m_gates[i]->onUpdate(i, true); // open gate
        } else {
            INFO("[" + std::to_string(this->m_portId) + "] gate [" + std::to_string(this->m_gates[i]->getPCP()) + "] -> 0");
            this->m_gates[i]->onUpdate(i, false); // close gate
        }
    }
    INFO("[" + std::to_string(this->m_portId) + "] cursor [" + std::to_string(this->m_currentCursor) + " -> [" + std::to_string(this->m_nextCursor) + "]");
    this->m_currentCursor = this->m_nextCursor;
    this->m_nextCursor = (this->m_currentCursor + 1) % this->m_length; // move to next item
}
    
} // namespace faker_tsn
