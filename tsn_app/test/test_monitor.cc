#include <faker_tsn.h>
#include <gtest/gtest.h>

#include "../src/utils/container/Trie.h"
#include "../src/utils/monitor/DataSpot.h"

using namespace faker_tsn;

static void TestTrie() {
    int a = 10;
    double d = 1.23;
    const char* s = "hello";
    struct _C {
        int a;
        int b;
    } c {10, 10};

    Trie* tree = new Trie("/");
    tree->insert("switch1.port0.queue0", (void*)&a, 1, 1, sizeof(int)); // int
    tree->insert("switch1.port1.queue0", (void*)&d, 1, 1, sizeof(double)); // double
    tree->insert("switch1.port1.queue1", (void*)s, strlen(s) + 1, strlen(s) + 1, sizeof(char)); // char*
    tree->insert("switch1.port2.queue0", (void*)&c, 1, 1, sizeof(struct _C)); // struct

    tree->print();

    // integer
    void* b;
    size_t len;
    size_t used;
    tree->search("switch1.port0.queue0", &b, &len, &used);
    for (int i = 0; i < len * sizeof(int); i++) {
        fprintf(stdout, "%02X ", ((char*)b)[i]);
    }
    fprintf(stdout, "\n");
    int aa = *((int*)b);
    fprintf(stdout, "%d\n", aa);

    // double
    tree->search("switch1.port1.queue0", &b, &len, &used);
    for (int i = 0; i < len * sizeof(double); i++) {
        fprintf(stdout, "%02X ", ((char*)b)[i]);
    }
    fprintf(stdout, "\n");
    double dd = *((double*)b);
    fprintf(stdout, "%lf\n", dd);
    
    // char*
    tree->search("switch1.port1.queue1", &b, &len, &used);
    for (int i = 0; i < len * sizeof(char); i++) {
        fprintf(stdout, "%02X ", ((char*)b)[i]);
    }
    fprintf(stdout, "\n");
    char* ss = (char*)b;
    fprintf(stdout, "%s\n", ss);

    // struct
    tree->search("switch1.port2.queue0", &b, &len, &used);
    for (int i = 0; i < len * sizeof(struct _C); i++) {
        fprintf(stdout, "%02X ", ((char*)b)[i]);
    }
    fprintf(stdout, "\n");
    struct _C cc = *((struct _C*)b);
    fprintf(stdout, "%d ", cc.a);
    fprintf(stdout, "%d\n", cc.b);
}

static void TestAdd() {
    static DataSpot ds;

    ds.add<int>(1, {"switch1", "port0", "queue0", "frame_num"}); // int
    ds.add<double>(1.23, {"switch1", "port1", "queue0", "frame_num"}); // double
    ds.append<int>(1, {"switch1", "port0", "queue0", "buf_size"});
    ds.append<int>(2, {"switch1", "port0", "queue0", "buf_size"});

    DataSpot::tree->print();

    void* buf;
    size_t len = 0;
    size_t used = 0;
    DataSpot::tree->search("switch1.port0.queue0.buf_size", &buf, &len, &used);
    int* arr = (int*)buf;
    for (int i = 0; i < len; i++) {
        fprintf(stdout, "%d ", *(arr + i));
    }
    fprintf(stdout, "\n");
}

TEST(TEST_MONITOR, TEST_TRIE) {
    // TestTrie();
    TestAdd();
}