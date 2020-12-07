#include <faker_tsn.h>
#include <gtest/gtest.h>
#include <linux/if_packet.h>
#include <net/if.h>
#include <netinet/if_ether.h>
#include <pcap/pcap.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "../src/networking/if_ether.h"
#include "../src/utils/Log.h"
#include "../src/utils/container/ConvertUtils.h"

using namespace faker_tsn;

static void TestSend() {
    union ethframe {
        struct
        {
            struct ethhdr header;
            unsigned char data[ETH_DATA_LEN];
        } field;
        unsigned char buffer[ETH_FRAME_LEN];
    };  // ethframe

    // const char* deviceName = "eth1";
    const char* deviceName = "h1-eth0";
    // const char* deviceName = "ens33";

    /* create raw socket */
    int sockfd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_8021Q));  // create socket

    /* get interface index */
    int ifindex = LinkLayerInterface::getIndex(deviceName);

    /* get interface mac address */
    unsigned char mac[8];
    LinkLayerInterface::getMacAddress(deviceName)->getRaw(mac);

    int frameNum = 1;
    for (int i = 1; i <= frameNum; i++) {
        INFO("Encode frame");
        /* construct ethernet header */
        struct ethhdr eth_hdr;
        memset(&eth_hdr, 0x00, sizeof(eth_hdr));
        unsigned char dest[ETH_ALEN] = {0x01, 0x00, 0x5E, 0x00, 0x00, 0x01};
        memcpy(&eth_hdr.h_dest, dest, ETH_ALEN);   // set dest mac
        memcpy(&eth_hdr.h_source, mac, ETH_ALEN);  // set src mac
        // don't set ETH_P_8021Q, it will ceause receiver to discard vlan tag
        // eth_hdr.h_proto = htons(ETH_P_8021Q);  // set IEEE 802.1Q protocol
        eth_hdr.h_proto = htons(ETH_P_ALL);  // set IEEE 802.1Q protocol
        INFO("dest mac = " + ConvertUtils::converBinToHexString(reinterpret_cast<unsigned char*>(&eth_hdr.h_dest), 6));
        INFO("src mac = " + ConvertUtils::converBinToHexString(reinterpret_cast<unsigned char*>(&eth_hdr.h_source), 6));
        INFO("protocol = " + ConvertUtils::converBinToHexString(reinterpret_cast<unsigned char*>(&eth_hdr.h_proto), 2));

        /* construct VLAN-tag */
        // struct vlan_tci tci = 0x1001;
        // memset(&tci, 0x00, sizeof(tci));
        // tci.pcp = 7;
        // tci.vid = 1;
        // INFO("pcp = " + std::to_string(tci.pcp));
        // INFO("dei = " + std::to_string(tci.dei));
        // INFO("vid = " + std::to_string(tci.vid));
        __be16 tci = htons(0xE001);  // 0010 0000 0000 0001
        struct vlan_hdr vlan_tag;
        memset(&vlan_tag, 0x00, sizeof(vlan_tag));
        memcpy(&vlan_tag.h_vlan_TCI, &tci, sizeof(tci));        // set TCI
        // vlan_tag.h_vlan_encapsulated_proto = htons(ETH_P_ALL);  // set IEEE 1722 protocol
        vlan_tag.h_vlan_encapsulated_proto = htons(ETH_P_TSN);  // set IEEE 1722 protocol
        INFO("TCI = " + ConvertUtils::converBinToHexString(reinterpret_cast<unsigned char*>(&vlan_tag.h_vlan_TCI), 2));
        INFO("protocol = " + ConvertUtils::converBinToHexString(reinterpret_cast<unsigned char*>(&vlan_tag.h_vlan_encapsulated_proto), 2));

        /* construct R-tag */
        struct rtag_hdr rtag;
        memset(&rtag, 0x00, sizeof(rtag));
        rtag.h_rtag_seq_num = htons(i);
        rtag.h_rtag_encapsulated_proto = htons(ETH_P_IP);
        INFO("reserved = " + ConvertUtils::converBinToHexString(reinterpret_cast<unsigned char*>(&rtag.h_rtag_rsved), 2));
        INFO("sequence number = " + ConvertUtils::converBinToHexString(reinterpret_cast<unsigned char*>(&rtag.h_rtag_seq_num), 2));
        INFO("protocol = " + ConvertUtils::converBinToHexString(reinterpret_cast<unsigned char*>(&rtag.h_rtag_encapsulated_proto), 2));

        /* construct TSN frame */
        union tsn_frame frame;
        const char* data = "hello world\n";
        unsigned int frame_len = strlen(data) + ETH_HLEN + 4 + 6;
        memset(&frame, 0x00, sizeof(tsn_frame));
        INFO("TSN frame length = " + std::to_string(sizeof(frame)));
        memcpy(&frame.filed.header.eth_hdr, &eth_hdr, sizeof(eth_hdr));
        memcpy(&frame.filed.header.vlan_tag, &vlan_tag, sizeof(vlan_tag));
        memcpy(&frame.filed.header.r_tag, &rtag, sizeof(rtag));
        memcpy(frame.filed.data, data, strlen(data));
        INFO("data = " + std::string(data));

        /* fill in frame */
        // union ethframe frame;
        // unsigned char dest[ETH_ALEN] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};  // src mac
        // const char* data = "hello world\n";
        // unsigned short data_len = strlen(data);
        // unsigned int frame_len = data_len + ETH_HLEN;
        // memcpy(frame.field.header.h_dest, dest, ETH_ALEN);   // set dest mac
        // memcpy(frame.field.header.h_source, mac, ETH_ALEN);  // set src mac
        // frame.field.header.h_proto = htons(ETH_P_ALL);       // set protocol
        // memcpy(frame.field.data, data, data_len);            // set data payload

        /* set linklayer address */
        struct sockaddr_ll saddrll;  // link layer socker address struct
        memset((void*)&saddrll, 0x00, sizeof(saddrll));
        saddrll.sll_family = PF_PACKET;           // set address family
        saddrll.sll_ifindex = ifindex;            // set interface index
        saddrll.sll_halen = ETH_ALEN;             // set address length
        saddrll.sll_protocol = htons(ETH_P_ALL);  // set protocol
        // memcpy((void*)(saddrll.sll_addr), (void*)dest, ETH_ALEN);  // set mac

        /* send data */
        if (sendto(sockfd, frame.buffer, frame_len, 0, (struct sockaddr*)&saddrll, sizeof(saddrll)) > 0)
            INFO("Send success!");
        else
            ERROR("Error, could not send");
    }

    close(sockfd);
}

TEST(TEST_SENDER, TEST_SENDER) {
    TestSend();
}