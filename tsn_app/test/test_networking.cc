#include <faker_tsn.h>
#include <gtest/gtest.h>

#include <string>

#include "../src/networking/VlanTCI.h"
#include "../src/networking/if_ether.h"
#include "../src/utils/container/ConvertUtils.h"

using namespace faker_tsn;

static void TestGetMulticastMacAddress() {
    int sockfd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_ALL));
    struct ifreq buffer;
    int ifindex;
    memset(&buffer, 0x00, sizeof(buffer));
    strncpy(buffer.ifr_name, "ens33", IFNAMSIZ);
    if (ioctl(sockfd, SIOCADDMULTI, &buffer) < 0) {
        printf("Error: could not get interface index\n");
        // TODO handle error
    }
    ifindex = buffer.ifr_ifindex;
}

static void TestGetMacAddress() {
    try {
        MacAddress* macAddress = LinkLayerInterface::getMacAddress("ens33");
        unsigned char rawMac[ETH_ALEN];
        macAddress->getRaw(rawMac);
        for (int i = 0; i < ETH_ALEN; i++) {
            printf("%d ", rawMac[i]);
        }
        printf("\n");
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
}

static void TestGetIndex() {
    try {
        int index = LinkLayerInterface::getIndex("ens33");
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
}

static void TestFindInterface() {
    try {
        LinkLayerInterface* interface = LinkLayerInterface::findInterface("ens33");
        INFO("\n" + interface->toString());
    } catch (const std::exception& e) {
        std::cerr << e.what() << '\n';
    }
}

static void TestIfEther() {
    const char* deviceName = "ens33";

    /* get interface index */
    int ifindex = LinkLayerInterface::getIndex(deviceName);

    /* get interface mac address */
    unsigned char src[8];
    LinkLayerInterface::getMacAddress(deviceName)->getRaw(src);

    // test Ethernet header
    struct ethhdr eth_hdr;
    INFO("Ethernet header length = " + std::to_string(sizeof(eth_hdr)));
    memset(&eth_hdr, 0x00, sizeof(eth_hdr));
    memcpy(&eth_hdr.h_source, src, ETH_ALEN);  // set src mac
    eth_hdr.h_proto = htons(ETH_P_8021Q);      // set IEEE 802.1Q protocol
    INFO("protocol = " + ConvertUtils::converBinToHexString(reinterpret_cast<unsigned char*>(&eth_hdr.h_proto), 2));

    // test TCI
    struct vlan_tci tci;
    INFO("TCI length = " + std::to_string(sizeof(tci)));
    memset(&tci, 0x00, sizeof(tci));
    tci.pcp = 1;
    tci.vid = 1;
    INFO("pcp = " + std::to_string(tci.pcp));
    INFO("dei = " + std::to_string(tci.dei));
    INFO("vid = " + std::to_string(tci.vid));
    INFO("TCI = " + ConvertUtils::converBinToHexString(reinterpret_cast<unsigned char*>(&tci), 2));

    __be16 _tci = htons(0xE001);  // 1110 0000 0000 0001
    INFO("n_TCI = " + ConvertUtils::converBinToHexString(reinterpret_cast<unsigned char*>(&_tci), 2));
    __be16 __tci = 0xE001;
    INFO("h_TCI = " + ConvertUtils::converBinToHexString(reinterpret_cast<unsigned char*>(&__tci), 2));

    VlanTCI vlanTCI = VlanTCI::parse(_tci);
    INFO("VlanTCI.pcp = " + std::to_string(vlanTCI.pcp));
    INFO("VlanTCI.dei = " + std::to_string(vlanTCI.dei));
    INFO("VlanTCI.vid = " + std::to_string(vlanTCI.vid));

    // test VLAN tag
    struct vlan_hdr vlan_tag;
    INFO("VLAN tag length = " + std::to_string(sizeof(vlan_tag)));
    memset(&vlan_tag, 0x00, sizeof(vlan_tag));
    memcpy(&vlan_tag.h_vlan_TCI, &_tci, sizeof(_tci));        // set TCI
    vlan_tag.h_vlan_encapsulated_proto = htons(ETH_P_8021Q);  // set IEEE 1722 protocol
    INFO("TCI = " + ConvertUtils::converBinToHexString(reinterpret_cast<unsigned char*>(&vlan_tag.h_vlan_TCI), 2));
    INFO("protocol = " + ConvertUtils::converBinToHexString(reinterpret_cast<unsigned char*>(&vlan_tag.h_vlan_encapsulated_proto), 2));

    // test R-tag
    struct rtag_hdr rtag;
    INFO("R-tag length = " + std::to_string(sizeof(rtag)));
    memset(&rtag, 0x00, sizeof(rtag));
    rtag.h_rtag_seq_num = htons(1);  // set
    rtag.h_rtag_encapsulated_proto = htons(ETH_P_ALL);
    INFO("reserved = " + ConvertUtils::converBinToHexString(reinterpret_cast<unsigned char*>(&rtag.h_rtag_rsved), 2));
    INFO("sequence number = " + ConvertUtils::converBinToHexString(reinterpret_cast<unsigned char*>(&rtag.h_rtag_seq_num), 2));
    INFO("protocol = " + ConvertUtils::converBinToHexString(reinterpret_cast<unsigned char*>(&rtag.h_rtag_encapsulated_proto), 2));

    // construct TSN frame
    union tsn_frame frame;
    char* data = "hello world\n";
    INFO("raw data = " + ConvertUtils::converBinToHexString(reinterpret_cast<unsigned char*>(data), 13));
    memset(&frame, 0x00, sizeof(tsn_frame));
    INFO("TSN frame length = " + std::to_string(sizeof(frame)));
    memcpy(&frame.filed.header.eth_hdr, &eth_hdr, sizeof(eth_hdr));
    memcpy(&frame.filed.header.vlan_tag, &vlan_tag, sizeof(vlan_tag));
    memcpy(&frame.filed.header.r_tag, &rtag, sizeof(rtag));
    memcpy(frame.filed.data, data, strlen(data));
    INFO("data = " + ConvertUtils::converBinToHexString(reinterpret_cast<unsigned char*>(frame.filed.data), 13));
}

static void TestMacTable() {
    MacTable::loadRouteXML("./config/routes.xml");
    INFO("\n" + MacTable::toString());
}

TEST(TEST_NETWORKING, TEST_NETWORKING_INTERFACE) {
    // TestGetMacAddress();
    // TestGetIndex();
    // TestFindInterface();
    // TestIfEther();
}

TEST(TEST_NETWORKING, TEST_MAC_TABLE) {
    TestMacTable();
}