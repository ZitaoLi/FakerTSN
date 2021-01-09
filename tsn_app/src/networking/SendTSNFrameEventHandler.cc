#include "SendTSNFrameEventHandler.h"

namespace faker_tsn {

SendTSNFrameEventHandler::SendTSNFrameEventHandler(
    HANDLE handle,
    struct sockaddr_ll& sockAddrII,
    IPort* port) : m_handle(handle), m_port(port) {
    this->m_isEnhanced = ConfigSetting::getInstance().get<bool>("enhancedGCL");
    memcpy(&this->m_sockAddrII, &sockAddrII, sizeof(sockAddrII));
}

SendTSNFrameEventHandler::~SendTSNFrameEventHandler() {
}

HANDLE SendTSNFrameEventHandler::getHandle() {
    return this->m_handle;
}

void SendTSNFrameEventHandler::handle_event(EVENT_TYPE eventType) {
    assert(eventType == EVENT_TYPE::WRITE);

    INFO("SendTSNFrameEventHandler on call");

    /* try to get frame from queue */
    IFrameBody* frameBody = (TSNFrameBody*)this->m_port->output();
    if (frameBody == nullptr) {
        // disable EPOLLOUT event
        Reactor::getInstance().getDemultoplexer().updateHandle(this->m_handle, 0x00);
        INFO("Nothing to be sent");
        return;
    }

    INFO("Encode frame");
    INFO("------------- TSN frame  --------------");
    
    unsigned char dest[ETH_ALEN] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    if (!this->m_isEnhanced) {
        auto it = MacTable::peers.find(this->m_port->getDeviceID());
        if (it == MacTable::peers.end()) {
            ERROR("invalid peer mac");
            return;
        }
        memcpy(dest, it->second, ETH_ALEN);
    } else {
        EnhancementTSNFrameBody* _f = dynamic_cast<EnhancementTSNFrameBody*>(frameBody);
        _f->getMac(dest);
    }

    /* construct ethernet header */
    struct ethhdr eth_hdr;
    memset(&eth_hdr, 0x00, sizeof(eth_hdr));
    memcpy(&eth_hdr.h_dest, dest, ETH_ALEN);                           // set dest mac
    memcpy(&eth_hdr.h_source, this->m_sockAddrII.sll_addr, ETH_ALEN);  // set src mac
    eth_hdr.h_proto = htons(ETH_P_ALL);                                // set IEEE 802.1Q protocol
    INFO("dest mac = " + ConvertUtils::converBinToHexString(reinterpret_cast<unsigned char*>(&eth_hdr.h_dest), 6));
    INFO("src mac = " + ConvertUtils::converBinToHexString(reinterpret_cast<unsigned char*>(&eth_hdr.h_source), 6));
    INFO("protocol = " + ConvertUtils::converBinToHexString(reinterpret_cast<unsigned char*>(&eth_hdr.h_proto), 2));

    __be16 tci;
    __be16 rsv;
    __be16 seq;
    struct vlan_hdr vlan_tag;
    memset(&vlan_tag, 0x00, sizeof(vlan_tag));
    struct rtag_hdr rtag;
    memset(&rtag, 0x00, sizeof(rtag));
    if (!this->m_isEnhanced) {
        TSNFrameBody* _f = dynamic_cast<TSNFrameBody*>(frameBody);
        VlanTCI vlan_tci;
        vlan_tci.pcp = _f->getPCP();
        vlan_tci.vid = _f->getVID();
        tci = VlanTCI::encode(vlan_tci);
        seq = _f->getSeq();
    } else {
        EnhancementTSNFrameBody* _f = dynamic_cast<EnhancementTSNFrameBody*>(frameBody);
        VlanTCI vlan_tci;
        vlan_tci.pcp = _f->getPCP();
        vlan_tci.vid = _f->getVID();
        tci = VlanTCI::encode(vlan_tci);
        seq = _f->getSeq();
        RTCI r_tci;
        r_tci.fid = _f->getFlowId();
        r_tci.phs = _f->getPhase();
        rsv = RTCI::encode(r_tci);
    }
    memcpy(&vlan_tag.h_vlan_TCI, &tci, sizeof(tci));        // set TCI
    vlan_tag.h_vlan_encapsulated_proto = htons(ETH_P_TSN);  // set IEEE 1722 protocol
    rtag.h_rtag_rsved = rsv;                                // set reserved
    rtag.h_rtag_seq_num = htons(seq);                       // set seq
    rtag.h_rtag_encapsulated_proto = htons(ETH_P_IP);       // set IP Protocol 

    INFO("TCI = " + ConvertUtils::converBinToHexString(reinterpret_cast<unsigned char*>(&vlan_tag.h_vlan_TCI), 2));
    INFO("protocol = " + ConvertUtils::converBinToHexString(reinterpret_cast<unsigned char*>(&vlan_tag.h_vlan_encapsulated_proto), 2));
    INFO("reserved = " + ConvertUtils::converBinToHexString(reinterpret_cast<unsigned char*>(&rtag.h_rtag_rsved), 2));
    INFO("sequence number = " + ConvertUtils::converBinToHexString(reinterpret_cast<unsigned char*>(&rtag.h_rtag_seq_num), 2));
    INFO("protocol = " + ConvertUtils::converBinToHexString(reinterpret_cast<unsigned char*>(&rtag.h_rtag_encapsulated_proto), 2));

    /* construct VLAN-tag */
    // VlanTCI vlan_tci;
    // vlan_tci.pcp = frameBody->getPCP();
    // vlan_tci.vid = frameBody->getVID();
    // __be16 tci = VlanTCI::encode(vlan_tci);
    // struct vlan_hdr vlan_tag;
    // memset(&vlan_tag, 0x00, sizeof(vlan_tag));
    // memcpy(&vlan_tag.h_vlan_TCI, &tci, sizeof(tci));        // set TCI
    // // vlan_tag.h_vlan_encapsulated_proto = htons(ETH_P_ALL);  // set IEEE 1722 protocol
    // vlan_tag.h_vlan_encapsulated_proto = htons(ETH_P_TSN);  // set IEEE 1722 protocol
    // INFO("TCI = " + ConvertUtils::converBinToHexString(reinterpret_cast<unsigned char*>(&vlan_tag.h_vlan_TCI), 2));
    // INFO("protocol = " + ConvertUtils::converBinToHexString(reinterpret_cast<unsigned char*>(&vlan_tag.h_vlan_encapsulated_proto), 2));

    /* construct R-tag */
    // struct rtag_hdr rtag;
    // __be16 seq = frameBody->getSeq();
    // memset(&rtag, 0x00, sizeof(rtag));
    // rtag.h_rtag_seq_num = htons(seq);  // set
    // rtag.h_rtag_encapsulated_proto = htons(ETH_P_IP);
    // if (this->m_isEnhanced) {
    //     RTCI r_tci;
    //     EnhancementTSNFrameBody* _f = dynamic_cast<EnhancementTSNFrameBody*>(frameBody);
    //     r_tci.fid = _f->getFlowId();
    //     r_tci.phs = _f->getPhase();
    //     __be16 rsv = RTCI::encode(r_tci);
    //     rtag.h_rtag_rsved = rsv;
    // }  
    // INFO("reserved = " + ConvertUtils::converBinToHexString(reinterpret_cast<unsigned char*>(&rtag.h_rtag_rsved), 2));
    // INFO("sequence number = " + ConvertUtils::converBinToHexString(reinterpret_cast<unsigned char*>(&rtag.h_rtag_seq_num), 2));
    // INFO("protocol = " + ConvertUtils::converBinToHexString(reinterpret_cast<unsigned char*>(&rtag.h_rtag_encapsulated_proto), 2));

    /* construct TSN frame */
    union tsn_frame frame;
    unsigned int data_len = frameBody->getBytes();
    unsigned char* data = (unsigned char*)malloc(data_len);
    frameBody->getData(data, data_len);
    unsigned int frame_len = data_len + ETH_HLEN + 4 + 6;
    memset(&frame, 0x00, sizeof(tsn_frame));
    INFO("TSN frame length = " + std::to_string(frame_len));
    memcpy(&frame.filed.header.eth_hdr, &eth_hdr, sizeof(eth_hdr));
    memcpy(&frame.filed.header.vlan_tag, &vlan_tag, sizeof(vlan_tag));
    memcpy(&frame.filed.header.r_tag, &rtag, sizeof(rtag));
    memcpy(frame.filed.data, data, data_len);
    INFO("data(string) = " + std::string((char*)frame.filed.data));

    /* send data */
    int fd = this->m_handle;
    // TODO frame size must not more than 1514
    frame_len = (frame_len > ETH_FRAME_LEN) ? ETH_FRAME_LEN : frame_len;
    if (sendto(fd, frame.buffer, frame_len, 0, (struct sockaddr*)&this->m_sockAddrII, sizeof(this->m_sockAddrII)) > 0)
        INFO("Send success!");
    else
        ERROR("Send error: " + std::string(strerror(errno)));
}

}  // namespace faker_tsn
