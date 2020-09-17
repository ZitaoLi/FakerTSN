#include "faker_tsn.h"
#include "test.h"

using namespace faker_tsn;

void test_logger() {
    Log::setLoggerLevel(Log::INFO_LEVEL);
    DEBUG("debug world");
    INFO("info world");
}