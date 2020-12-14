#include <faker_tsn.h>
#include <gtest/gtest.h>

#include <iostream>
using namespace faker_tsn;
using namespace std;

static void TestGateControlList() {
    GateControlList* gcl = dynamic_cast<GateControlList*>(REFLECTOR::CreateByTypeName(std::string("faker_tsn::GateControlList"), 1U));  // scope declaration is required

    TimeContext::getInstance().getTimer()->start();

    while (true)
    {
        /* code */
    }
    
}

static void TestGateControlListFull() {
    // crate port
    const char* name1 = "ens33";
    std::shared_ptr<IPort> port1 = std::make_shared<DataPort>(name1);
    std::shared_ptr<IPortState> creationState1 = std::make_shared<CreationPortState>();
    creationState1->doAction(port1);

    // add port into port manager
    std::shared_ptr<PortManager> portManager = TSNContext::getInstance().getPortManager();
    portManager->appendPort(port1);

    // start
    TimeContext::getInstance().getTimer()->start();

    while (true)
    {
        /* code */
    }
}

static void TestIEEE8021QbvQueue() {
    // before run the program, pls modify config.ini to switch.queue.class=IEEE8021QbvQueue

    // crate port
    const char* name1 = "ens33";
    std::shared_ptr<IPort> port1 = std::make_shared<DataPort>(name1);
    std::shared_ptr<IPortState> creationState1 = std::make_shared<CreationPortState>();
    creationState1->doAction(port1);

    // create a frame
    unsigned char data[100] = "hello world";
    TSNFrameBody* iFrame = new TSNFrameBody();
    iFrame->setPCP(7);
    iFrame->setVID(1);
    iFrame->setSeq(0);
    iFrame->setType(IEEE_802_1Q_TSN_FRAME_E);
    iFrame->setData(data, 100);

    // input frame into queue
    port1->input((void*)iFrame, 100, IEEE_802_1Q_TSN_FRAME_E);

    // output frame from queue
    std::shared_ptr<QueueContext> qContext = dynamic_pointer_cast<DataPort>(port1)->getQueueContext();
    IFrameBody* oFrame = qContext->dequeue();

    // start
    TimeContext::getInstance().getTimer()->start();

    while (true)
    {
        /* code */
    }
}

TEST(TEST_QUEUE, TEST_GATE_CONTROL_LIST) {
    // TestGateControlList();
    // TestGateControlListFull();
}

TEST(TEST_QUEUE, TEST_IEEE_8021QBV_QUEUE) {
    TestIEEE8021QbvQueue();
}