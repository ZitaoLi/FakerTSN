#include "RecvTSNFrameEventHandler.h"

namespace faker_tsn {

const __be16 RecvTSNFrameEventHandler::s_protocol = htons(ETH_P_TSN);

RecvTSNFrameEventHandler::RecvTSNFrameEventHandler(
    HANDLE handle, struct sockaddr_ll& sockAddrII) : m_handle(handle) {
    memcpy(&this->m_sockAddrII, &sockAddrII, sizeof(sockAddrII));
    this->m_isEnhanced = ConfigSetting::getInstance().get<bool>("enhancedGCL");
}

RecvTSNFrameEventHandler::~RecvTSNFrameEventHandler() {
}

void RecvTSNFrameEventHandler::handle_event(EVENT_TYPE eventType) {
    assert(eventType == EVENT_TYPE::READ);

    INFO("RecvTSNFrameEventHandler on call");

    /* receive raw data */
    unsigned char recvbuf[ETH_FRAME_LEN + 4 + 6];
    int fd = this->m_handle;
    if (recv(fd, recvbuf, ETH_FRAME_LEN + 4 + 6, 0) > 0) {
        INFO("Recv success!");
    } else {
        INFO("Recv error!");
    }

    /* filter ring packet */
    /* because the frame send to interface will be receive again */
    if (memcmp(recvbuf + 6, this->m_sockAddrII.sll_addr, 6) == 0) {
        // INFO("------------- ring frame --------------");
        return;
    }

    // TODO handle un-matched dest mac
    // if (memcmp(recvbuf, this->m_sockAddrII.sll_addr, 6) != 0) {
    //     INFO("------------- Non-TSN frame --------------");
    //     return;
    // } else {
    //     INFO("------------- TSN frame  --------------");
    // }

    /* filter non-TSN frame */
    if (memcmp(recvbuf + 16, &RecvTSNFrameEventHandler::s_protocol, 2) != 0) { // security issue may occur here
        INFO("------------- Non-TSN frame --------------");
        // TODO handle using non-TSN method
        return;
    } else {
        INFO("------------- TSN frame  --------------");
    }

    INFO("Decode frame");

    // ethernet header
    INFO("dest mac = " + ConvertUtils::converBinToHexString((recvbuf), 6));
    INFO("src mac = " + ConvertUtils::converBinToHexString((recvbuf) + 6, 6));
    INFO("ether protocol = " + ConvertUtils::converBinToHexString((recvbuf) + 12, 2));
    // vlan-tag
    INFO("TCI = " + ConvertUtils::converBinToHexString((recvbuf) + 14, 2));
    INFO("vlan-tag protocol = " + ConvertUtils::converBinToHexString((recvbuf) + 16, 2));
    // r-tag
    INFO("reserved = " + ConvertUtils::converBinToHexString((recvbuf) + 18, 2));
    INFO("sequence number = " + ConvertUtils::converBinToHexString((recvbuf) + 20, 2));
    INFO("r-tag protocol = " + ConvertUtils::converBinToHexString((recvbuf) + 22, 2));
    // INFO("data = " + ConvertUtils::converBinToHexString((recvbuf) + 24, ETH_DATA_LEN));
    INFO("data(string) = " + std::string(reinterpret_cast<char*>(recvbuf) + 24));

    /* parse raw data to tsn frame */
    unsigned char destMac[ETH_ALEN];
    unsigned char srcMac[ETH_ALEN];
    memcpy(destMac, recvbuf, ETH_ALEN);
    memcpy(srcMac, recvbuf + 6, ETH_ALEN);
    IFrameBody* frame;
    RELAY_ENTITY type;
    size_t len;
    __be16 tci;
    memcpy(&tci, recvbuf + 14, 2);
    VlanTCI vlan_tci = VlanTCI::parse(tci);
    __be16 seq;
    memcpy(&seq, recvbuf + 20, 2);
    if (!this->m_isEnhanced) {
        type = RELAY_ENTITY::IEEE_802_1Q_TSN_FRAME;
        frame = new TSNFrameBody();
        TSNFrameBody* _f = dynamic_cast<TSNFrameBody*>(frame);
        len = sizeof(_f);
        _f->setPCP(vlan_tci.pcp);
        _f->setVID(vlan_tci.vid);
        _f->setSeq(ntohs(seq));
    } else {
        type = RELAY_ENTITY::IEEE_802_1Q_TSN_FRAME_E;
        frame = new EnhancementTSNFrameBody();
        EnhancementTSNFrameBody* _f = dynamic_cast<EnhancementTSNFrameBody*>(frame);
        len = sizeof(_f);
        _f->setPCP(vlan_tci.pcp);
        _f->setVID(vlan_tci.vid);
        _f->setSeq(ntohs(seq));
        _f->setMac(destMac);
        memcpy(&tci, recvbuf + 18, 2);
        RTCI r_tci = RTCI::parse(tci);
        _f->setFlowId(r_tci.fid);
        _f->setPhse(r_tci.phs);
    }
    frame->setData(recvbuf + 24, strlen((char*)recvbuf + 24));

    /* forward frame */
    ForwardFunction::forward(srcMac, destMac, reinterpret_cast<void*>(frame), len, type);
}

HANDLE RecvTSNFrameEventHandler::getHandle() {
    return this->m_handle;
}

}  // namespace faker_tsn
