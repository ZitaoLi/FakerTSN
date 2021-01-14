#ifndef _DATA_SPOT_H
#define _DATA_SPOT_H

#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <iomanip>
#include <initializer_list>

#include "../container/Trie.h"

namespace faker_tsn
{

class DataSpot {
public:
    static Trie* tree;

    static std::string parse(std::initializer_list<std::string>& strs) {
        std::stringstream ss;
        for (std::string str: strs) {
            ss << str << ".";
        }
        std::string words = ss.str();
        words = words.substr(0, words.length() - 1);
        return words;
    }

    /* basic type */
    template<typename T>
    static void add(const T &val, std::initializer_list<std::string> strs) {
        /* get parameters */
        std::string words = parse(strs);

        void* buf;
        size_t len = 0;
        size_t used = 0;
        tree->search(words, &buf, &len, &used);
        T t = val;
        if (len != 0) {
            t = *((T*)buf);
            t += val;
        }
        tree->insert(words, (void*)&t, 1, 1, sizeof(t));
    }

    /* array for basic type */
    template<typename T>
    static void append(const T &val, std::initializer_list<std::string> strs) {
        /* get parameters */
        std::string words = parse(strs);

        void* buf;
        size_t len = 0;
        size_t used = 0;
        tree->search(words, &buf, &len, &used);
        T t = val;
        tree->update(words, (void*)&t, 1, used, sizeof(t));
    }

    static void persist(std::string filename);
};
    
} // namespace faker_tsn

#endif // _DATA_SPOT_H