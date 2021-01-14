#ifndef _TRIE_H
#define _TRIE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#include "StringHelper.h"

namespace faker_tsn
{

class Trie {
public:
    /* node key */
    std::string _key;
    /* next node */
    std::vector<Trie*> _next;
    /* node value */
    void* _data;
    size_t _len;
    size_t _size;
    size_t _used;

public:
    Trie(std::string key) : _key(key), _data(nullptr), _len(0), _size(1), _used(0) {
    }

    ~Trie() {
    }

    void print();

    void insert(std::string str, void* data, size_t len, size_t used, size_t size);

    void update(std::string str, void* data, size_t len, size_t used, size_t size);

    int search(std::string str, void** data, size_t* len, size_t* used);
};
    
} // namespace faker_tsn


#endif _TRIE_H