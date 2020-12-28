#include <faker_tsn.h>
#include <gtest/gtest.h>

#include <memory>

using namespace faker_tsn;

static void Switch() {
    // set logger
    LOG_LEVEL logLevel = ConfigSetting::getInstance().get<uint8_t>("log.level");
    std::string logPath = ConfigSetting::getInstance().get<std::string>("log.path");
    std::string logMode = ConfigSetting::getInstance().get<std::string>("log.mode");
    Log::setLogger(Log::INFO_LEVEL, logPath, logMode);

    // // crate port 1
    // const char* name1 = "s2-eth0";
    // // const char* name1 = "eth1";
    // std::shared_ptr<IPort> port1 = std::make_shared<DataPort>(name1);
    // std::shared_ptr<IPortState> creationState1 = std::make_shared<CreationPortState>();
    // creationState1->doAction(port1);

    // // crate port 2
    // const char* name2 = "s2-eth1";
    // // const char* name2 = "eth2";
    // std::shared_ptr<IPort> port2 = std::make_shared<DataPort>(name2);
    // std::shared_ptr<IPortState> creationState2 = std::make_shared<CreationPortState>();
    // creationState2->doAction(port2);

    // // crate port 3
    // const char* name3 = "s2-eth2";
    // // const char* name3 = "eth3";
    // std::shared_ptr<IPort> port3 = std::make_shared<DataPort>(name3);
    // std::shared_ptr<IPortState> creationState3 = std::make_shared<CreationPortState>();
    // creationState2->doAction(port3);

    // // add port into port manager
    // std::shared_ptr<PortManager> portManager = TSNContext::getInstance().getPortManager();
    // portManager->appendPort(port1);
    // portManager->appendPort(port2);
    // portManager->appendPort(port3);

    // std::shared_ptr<PortManager> portManager = TSNContext::getInstance().getPortManager();
    // const char* name1 = "s2-eth0";
    // const char* name2 = "s2-eth1";
    // const char* name3 = "s2-eth2";
    // portManager->appendDeviceName(name1);
    // portManager->appendDeviceName(name2);
    // portManager->appendDeviceName(name3);
    // portManager->createPortFromDeviceNameList();

    // // load mac table
    // MacTable::loadRouteXML("./config/routes.xml");
    // MulticastMacTable::loadRouteXML("./config/routes.xml");

    // // start timer
    // TimeContext::getInstance().getTimer()->start();

    // // enable reactor
    // Reactor::getInstance().handle_events();


    TSNContext& context = TSNContext::getInstance();
    INFO("TSN_CONTEXT_STATE: down -> init");
    std::shared_ptr<InitTSNContextState> initState = std::make_shared<InitTSNContextState>();
    initState->doAction(context);
}

TEST(TEST_H2_FORWARD, FORWARD1) {
    Switch();
}