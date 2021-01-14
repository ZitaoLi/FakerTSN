#include "Trie.h"

namespace faker_tsn {

void printHelper(Trie* node, std::string str, void* data, unsigned int len, size_t size) {
    if (node == nullptr) {
        fprintf(stdout, "%s = ", str.c_str());
        if (data) {
            for (int i = 0; i < len * size; i++) {
                fprintf(stdout, "%02X ", ((char*)data)[i]);
            }
            fprintf(stdout, "\n");
        }
        return;
    }
    str = str + "." + node->_key;
    if (node->_next.size() == 0) {
        printHelper(nullptr, str, node->_data, node->_len, node->_size);
    } else {
        for (Trie* t: node->_next) {
            printHelper(t, str, nullptr, 0, 0);
        }
    }
}

void Trie::print() {
    printHelper(this, "", nullptr, 0, 0);
}

void Trie::insert(std::string str, void* data, size_t len, size_t used, size_t size) {
    vector<std::string> words = StringHelper::split(str, ".");
    Trie* node = this;
    for (std::string word: words) {
        Trie* n = node;
        /* search next node */
        for (Trie* t: node->_next) {
            if (t->_key == word) {
                node = t;
                break;
            }
        }
        /* insert new node if not exist */
        if (node == n) {
            Trie* p = new Trie(word);
            node->_next.push_back(p);
            node = p;
        }
    }
    if (len != 0) {
        if (node->_data != NULL) 
            free(node->_data);
        node->_len = len;
        node->_used = used;
        node->_size = size;
        size_t n = len * size;
        node->_data = malloc(n);
        memset(node->_data, 0x00, n);
        memcpy(node->_data, data, n);
    }
}

void Trie::update(std::string str, void* data, size_t len, size_t offset, size_t size) {
    vector<std::string> words = StringHelper::split(str, ".");
    Trie* node = this;
    for (std::string word: words) {
        Trie* n = node;
        /* search next node */
        for (Trie* t: node->_next) {
            if (t->_key == word) {
                node = t;
                break;
            }
        }
        /* insert new node if not exist */
        if (node == n) {
            Trie* p = new Trie(word);
            node->_next.push_back(p);
            node = p;
        }
    }
    if (len != 0) {
        size_t n = len * size;
        size_t o = offset * size;
        if (node->_len == 0) {
            node->_len = len;
            node->_size = size;
            node->_data = malloc(n);
            memset(node->_data, 0x00, n);
            memcpy(node->_data, data, n);
        } else if (len + offset > node->_len) {
            /* relloc memory */
            size_t i = len + 99; // additional length
            size_t l = offset + i; // total length
            node->_data = realloc(node->_data, l * size);
            node->_len = l;
            memset((node->_data) + o, 0x00, i * size);
        }
        node->_used += len;
        memcpy((node->_data) + o, data, n);
    }
}

int Trie::search(std::string str, void** data, size_t* len, size_t* used) {
    vector<std::string> words = StringHelper::split(str, ".");
    Trie* node = this;
    for (std::string word: words) {
        Trie* n = node;
        for (Trie* t: node->_next) {
            if (t->_key == word) {
                node = t;
                break;
            }
        }
        if (node == n) {
            return -1;
        }
    }
    if (node->_len != 0) {
        size_t n = node->_len * node->_size;
        void* p = malloc(n);
        memset(p, 0x00, n);
        memcpy(p, node->_data, n);
        (*data) = p;
        (*len) = node->_len;
        (*used) = node->_used;
    }
    return 0;
}

}