#include <gtest/gtest.h>
#include <hello/hello.h>

#include <iostream>

using namespace std;

static int test_hello() {
    hello();

    return 1;
}

static void test_test() {
    cout << "test" << endl;
}

TEST(TEST_HELLO, TEST_HELLO) {
    EXPECT_EQ(1, test_hello());
}

TEST(TEST_HELLO, TEST_TEST) {
    test_test();
}