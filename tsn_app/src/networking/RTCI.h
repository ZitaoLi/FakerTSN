#ifndef _R_TCI_H
#define _R_TCI_H

#include <stdint.h>
#include <arpa/inet.h>

#include <sstream>
#include <string>

#include "if_ether.h"

namespace faker_tsn
{

struct RTCI {
    uint8_t fid;
    uint8_t phs;

    std::string toString() {
        std::stringstream ss;
        ss << "fid = " << fid << " ";
        ss << "phs = " << phs << " ";
        return ss.str();
    }

    static __be16 encode(RTCI r_tci) {
        __be16 _tci = 0x0000;

        _tci |= r_tci.fid;
        _tci <<= 8;
        _tci |= r_tci.phs;

        return htons(_tci);
    }

    static RTCI parse(__be16 tci) {
        RTCI r_tci;

        tci = ntohs(tci);

        r_tci.phs = tci & 0x00FF;
        tci >>= 8;
        r_tci.fid = tci & 0x00FF;

        return r_tci;
    }
};
    
} // namespace faker_tsn


#endif // _R_TCI_H