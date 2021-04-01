#include "CommandEventHandler.h"

#include "../core/port/ConsolePort.h"

namespace faker_tsn
{

CommandEventHandler::CommandEventHandler(HANDLE handle, struct sockaddr_ll* sockAddrII, IPort* port) : m_handle(handle), m_port(port) {
    this->m_deviceName = ConfigSetting::getInstance().get<std::string>("switch.port.cons");
    if (sockAddrII != nullptr)
        memcpy(&this->m_sockAddrII, sockAddrII, sizeof(struct sockaddr_ll));
}

CommandEventHandler::~CommandEventHandler() {

}

void CommandEventHandler::handle_input() {
    INFO("CommandEventHandler<IN> on call");

    std::string iCommand;

    if (this->m_deviceName == "stdin") {
        getline(std::cin, iCommand);
    } else if (this->m_deviceName == "fifo") {
        // echo -e 'run 2 10\c' > /tmp/faker_tsn_fifo
        FILE *fp = NULL;
        char buf[100];
        fp = fdopen(this->m_handle, "r");
        fgets(buf, 100, fp);
        iCommand = std::string(buf);
    } else {
        ERROR("unknow type of console port");
        exit(EXIT_FAILURE);
    }
    INFO("<<< " + iCommand);

    /* transform command */
    /* fortmat: [cmd] [arg1] [arg2] */
    // std::string iCommand(recvbuf);
    std::string oCommand;
    std::vector<std::string> keys(3);
    std::stringstream ss(iCommand);
    std::string _t;
    for (int i = 0; i < 3; i++) {
        std::getline(ss, _t, ' ');
        keys[i] = _t;
    } 
    if (keys[0] == "run") {
        // example: run 5 60
        INFO("set start time and runtime");
        std::string startTime = keys[1]; // seconds since the Epoch
        std::string runTime = keys[2]; // seconds
        Time::TimePoint now = Reflector::getNewInstance<IClock>("RealTimeClock")->now();
        Time::TimeInterval delay(std::stoi(keys[1]), 0);
        Time::TimePoint start = now + delay;
        INFO("start time: " + std::to_string(start.sec) + ", run time: " + keys[2]);
        oCommand += "run ";
        oCommand += std::to_string(start.sec);
        oCommand += " ";
        oCommand += keys[2];
        std::vector<std::shared_ptr<IPort>> ports = TSNContext::getInstance().getPortManager()->getPorts();
        for (auto port: ports) {
            port->input((void*)oCommand.c_str(), oCommand.length(), RELAY_ENTITY::PACKET_RAW);
        }
    } else if (keys[0] == "stop") {

    } else {
        // ignore
        return;
    }
    INFO(">>> " + oCommand);
}

void CommandEventHandler::handle_output() {
    INFO("CommandEventHandler<OUT> on call");

    void* data = this->m_port->output();
    if (!data) return;
    std::string command = *((std::string*)data);
    const char* buf = command.c_str();
    int len = strlen(buf);

    if (sendto(this->m_handle, buf, len, 0, (struct sockaddr*)&this->m_sockAddrII, sizeof(this->m_sockAddrII)) > 0)
        INFO("Send success!");
    else
        ERROR("Send error: " + std::string(strerror(errno)));
}

void CommandEventHandler::handle_event(EVENT_TYPE eventType) {
    if (eventType == EVENT_TYPE::READ) {
        this->handle_input();
    } else if (eventType == EVENT_TYPE::WRITE) {
        this->handle_output();
    } else {
        ERROR("invalid event");
    }
}
    
} // namespace faker_tsn
