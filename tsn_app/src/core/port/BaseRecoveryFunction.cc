#include "BaseRecoveryFunction.h"

namespace faker_tsn
{

IFrameBody* BaseRecoveryFunction::handle(IFrameBody* frame) {
    EnhancementTSNFrameBody* eFrame = dynamic_cast<EnhancementTSNFrameBody*>(frame);

    uint32_t fid = eFrame->getFlowId();
    uint16_t seq = eFrame->getSeq();

    INFO(toString() + " get frame: " + eFrame->toString());

    auto it = m_flowSeqMap.find(fid);
    if (it != m_flowSeqMap.end()) {
        frame_recorder fr = m_flowSeqMap[fid];
        bool isRepetitive = checkRepetitive(fr, seq);
        if(it != m_flowSeqMap.end()){
            m_flowSeqMap[fid] = fr;
        }
        if (isRepetitive == true) {
            INFO(toString() + " eliminate frame: " + eFrame->toString());
            delete(frame);
            return nullptr;
        }
    } else {
        INFO(toString() + " add flow " + std::to_string(fid));
        frame_recorder fr;
        FrameRecordEntry first = {seq, seq};
        fr.push_back(first);
        m_flowSeqMap.insert(std::pair<uint32_t, frame_recorder> {fid, fr});
    }
    return frame;
}

bool BaseRecoveryFunction::checkRepetitive(frame_recorder& fr, uint16_t seq) {
    INFO(toString() + " check repetitive");
    auto it = fr.begin();
    while (it != fr.end()) {
        int lower = (*it).lower;
        int upper = (*it).upper;
        if (seq >= lower && seq <= upper) {
            return true;
        } else if (seq == (upper + 1)) {
            ((*it).upper)++;
            auto old = it;
            if (++it != fr.end()) {
                if (seq == (*it).lower - 1) {
                    (*it).lower = lower;
                    it = fr.erase(old);
                }
            }
            return false;
        } else if (seq >= (upper + 2)) {
            if (++it != fr.end()) {
                if (seq == (*it).lower - 1) {
                    (*it).lower--;
                } else if(seq <= (*it).lower - 2) {
                    FrameRecordEntry mid = {seq, seq};
                    fr.insert(it, mid);
                } else {
                    continue;
                }
            } else {
                FrameRecordEntry last = {seq, seq};
                fr.push_back(last);
            }
            return false;
        } else {
            return true;
        }
    }
    return true;
}

} // namespace faker_tsn
