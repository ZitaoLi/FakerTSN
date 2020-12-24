#include "Logger.h"
#include "./reflector/DynamicCreate.h"

namespace faker_tsn
{

const uint8_t Logger::DEBUG = 1;
const uint8_t Logger::INFO = 2;
const uint8_t Logger::WARN = 3;
const uint8_t Logger::ERROR = 4;

Logger::Logger()
{
    this->m_level = Logger::INFO;
    this->m_path = "./log/log";
    this->m_mode = "std";
    this->os.open(this->m_path, std::ios::out | std::ios::trunc);
    this->prompts.insert({Logger::DEBUG, std::string("[DEBUG] ")});
    this->prompts.insert({Logger::INFO, std::string("[INFO] ")});
    this->prompts.insert({Logger::WARN, std::string("[WARN] ")});
    this->prompts.insert({Logger::ERROR, std::string("[ERROR] ")});
}

Logger::~Logger()
{
    fflush(stdout);
    this->os.flush();
    this->os.close();
}

void Logger::setLogger(uint8_t level, std::string path, std::string mode) {
    this->m_level = level;
    this->m_path = path;
    this->m_mode = mode;
}

void Logger::log(uint8_t level, const char* message, const char* filename, int line)
{
    if (this->m_level >= level) {
        // this->write(message, filename, line);
        if (this->m_mode == "file") {
            this->os << this->prompts[this->m_level] << filename << ":" << line << ": " << message << "\n";
        } else {
            std::cout << this->prompts[this->m_level] << filename << ":" << line << ": " << message << "\n";
        }
        return;
    }
    if (m_nextLogger != nullptr) {
        this->m_nextLogger->log(level, message, filename, line);
    }
}
    
} // namespace faker_tsn
