#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <pcap.h> /* GIMME a libpcap plz! */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <sys/socket.h>

#include "faker_tsn.h"
#include "test.h"

using namespace faker_tsn;

DataSpot ds;

int main(int argc, char **argv) {
    /* parse parameters */
    std::string filename;
    int opt;
    while ((opt = getopt(argc, argv, "hf:")) != -1) {
        switch (opt) {
            case 'h':
                printf("Usage: ./bin/tsn_app -f <InputFile>\n");
                break;
            case 'f':
                filename = optarg;
                break;
            case '?':
                printf("Usage: ./bin/tsn_app [-f <InputFile>]\n");
                break;
        }
    }
    if (filename.size() == 0) 
        exit(EXIT_FAILURE);

    /* print logo */
    printf("FAKER TSN\n\n\n");

    /* load ini file */
    ConfigSetting::getInstance().load(filename);

    /* set logger */
    LOG_LEVEL logLevel = ConfigSetting::getInstance().get<uint8_t>("log.level");
    std::string logPath = ConfigSetting::getInstance().get<std::string>("log.path");
    std::string logMode = ConfigSetting::getInstance().get<std::string>("log.mode");
    Log::setLogger(Log::INFO_LEVEL, logPath, logMode);

    /* work mode: host/switch */
    std::string mode = ConfigSetting::getInstance().get<std::string>("workMode");
    if (mode == "host") {
        TSNHost endSystem;
        endSystem.run();
    } else if (mode == "switch") {
        TSNSwitch relaySystem;
        relaySystem.run();
    } else if (mode == "configurator") {
        TSNConfigurator controlSystem;
        controlSystem.run();
    } else {
        ERROR("invalid work mode");
        exit(EXIT_FAILURE);
    }

    return 0;
}
