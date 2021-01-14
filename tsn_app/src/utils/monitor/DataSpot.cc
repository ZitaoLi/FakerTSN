#include "DataSpot.h"

namespace faker_tsn
{

Trie* DataSpot::tree = new Trie("/");

inline unsigned int to_uint(char ch) {
    return static_cast<unsigned int>(static_cast<unsigned char>(ch));
}

void persistHelper(std::ofstream& os, Trie* node, std::string str, void* data, unsigned int len, size_t size) {
    if (node == nullptr) {
        os << std::hex;
        os << str << "=";
        if (data) {
            char* buf = (char*)malloc(len * (size + 1));
            for (int i = 0; i < len * size; i++) {
                // sprintf(buf, "%02X ", ((char*)data)[i]);
                os << std::setw(2) << std::setfill('0') << to_uint(((char*)data)[i]) << " ";
            }
            // os << buf;
            os << "\n";
            free(buf);
        }
        return;
    }
    str = str + "." + node->_key;
    if (node->_next.size() == 0) {
        persistHelper(os, nullptr, str, node->_data, node->_len, node->_size);
    } else {
        for (Trie* t: node->_next) {
            persistHelper(os, t, str, nullptr, 0, 0);
        }
    }
}

void DataSpot::persist(std::string filename) {
    std::ofstream os;
    os.open(filename, std::ios::out | std::ios::trunc);

    persistHelper(os, DataSpot::tree, "", nullptr, 0, 0);

    os.flush();

    os.close();
}
    
} // namespace faker_tsn
