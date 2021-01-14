#ifndef _STRING_HELPER_H
#define _STRING_HELPER_H

#include <vector>
#include <string>

using namespace std;

namespace faker_tsn
{

class StringHelper {
public:
    static vector<string> split(const string &str, const string &pattern){
        vector<string> res;
        if(str == "")
            return res;
        string strs = str + pattern;
        size_t pos = strs.find(pattern);

        while(pos != strs.npos) {
            string temp = strs.substr(0, pos);
            res.push_back(temp);
            strs = strs.substr(pos+1, strs.size());
            pos = strs.find(pattern);
        }

        return res;
    }
};
    
} // namespace faker_tsn


#endif // STRING_HELPER_H