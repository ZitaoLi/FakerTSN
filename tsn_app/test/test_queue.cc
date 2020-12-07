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

TEST(TEST_QUEUE, TEST_GATE_CONTROL_LIST) {
    TestGateControlList();
}