#include <faker_tsn.h>
#include <gtest/gtest.h>

#include <string>

using namespace faker_tsn;

static void TestInitialization() {
    ConfigSetting& cs = ConfigSetting::getInstance();
    std::cout << cs << std::endl;

    const char* deviceName = cs.get<const char*>("deviceName");
    INFO(deviceName);

    std::string className = cs.get<std::string>("class");
    IClock* clock = Reflector::getNewInstance<IClock>("RealTimeClock");

    int port = cs.get<int>("port");
    INFO(std::to_string(port));
    cs.set("port", 8080);
    port = cs.get<int>("port");
    INFO(std::to_string(port));

    std::string objectName = "DefaultObject1";
    objectName = cs.get("object", objectName);
    INFO(objectName);

    objectName = cs.get<std::string>("object", "DefaultObject2");
    INFO(objectName);
}

TEST(TEST_CONFIG_SETTING, TEST_INITIALIZATION) {
    TestInitialization();
}