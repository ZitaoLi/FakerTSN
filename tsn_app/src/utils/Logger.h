#ifndef _LOGGER_H
#define _LOGGER_H

#include <stdint.h>
#include <memory>
#include <iostream>
#include <fstream>
#include <map>

namespace faker_tsn
{
    
class Logger
{
private:
    std::ofstream os;
    std::map<uint8_t, std::string> prompts;
    
protected:
    uint8_t m_level;
    std::string m_path;
    std::string m_mode;
    std::unique_ptr<Logger> m_nextLogger;

public:
    static const uint8_t DEBUG;
    static const uint8_t INFO;
    static const uint8_t WARN;
    static const uint8_t ERROR;

    Logger();

    virtual ~Logger();
    
    virtual void write(const char*, const char*, int){};

    void log(uint8_t level, const char*, const char*, int);

    void setLogger(uint8_t, std::string, std::string);
};

} // namespace faker_tsn


#endif // _LOGGER_H