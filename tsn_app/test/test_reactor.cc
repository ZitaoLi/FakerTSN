#include <gtest/gtest.h>
#include "faker_tsn.h"
#include "test.h"

using namespace faker_tsn;

static void TestReactor() {
    std::shared_ptr<IPort> port = std::make_shared<ConsolePort>();  // create port

    port->registerEventHandler();
    Reactor::getInstance().handle_events();
}

TEST(TEST_REACTOR, TEST_REACTOR) {
    TestReactor();
}