#ifndef _I_NODE_H
#define _I_NODE_H

#include <string>

namespace faker_tsn
{

class INode
{
public:
    virtual ~INode() = default;

    virtual void run() = 0;

    virtual std::string toString() { return ""; }
};

    
} // namespace faker_tsn


#endif // _I_NODE_H