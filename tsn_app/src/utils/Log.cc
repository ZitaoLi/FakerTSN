#include "Log.h"

namespace faker_tsn
{

uint8_t Log::INFO_LEVEL = Logger::INFO;
uint8_t Log::DEBUG_LEVEL = Logger::DEBUG;
uint8_t Log::WARN_LEVEL = Logger::WARN;
uint8_t Log::ERROR_LEVEL = Logger::ERROR;
uint8_t Log::level = Logger::INFO;
std::string Log::path = "./log/log";
std::string Log::mode = "std";
Logger* Log::m_logger = new Logger();

Logger* Log::getLogger()
{
    // static DebugLogger m_logger;
    return Log::m_logger;
}

void Log::setLogger(uint8_t level, std::string path, std::string mode) {
    Log::level = level;
    Log::path = path;
    Log::mode = mode;

    if (mode == "std" || mode == "file") {
        Log::m_logger->setLogger(level, path, mode);       
    } else {
        ERROR("invalid log mode");
        exit(EXIT_FAILURE);
    }
}

void Log::setLoggerLevel(const uint8_t& level)
{
    Log::level = level;
}

void Log::info(const char* message, const char* filename, int line)
{
    if (Log::level <= Log::INFO_LEVEL) {
        Log::m_logger->log(Log::INFO_LEVEL, message, filename, line);
    }
}

void Log::info(const std::string& message, const char* filename, int line)
{
    if (Log::level <= Log::INFO_LEVEL) {
        Log::m_logger->log(Log::INFO_LEVEL, message.c_str(), filename, line);
    }
}

void Log::debug(const char* message, const char* filename, int line)
{
    if (Log::level <= Log::DEBUG_LEVEL) {
        Log::m_logger->log(Log::DEBUG_LEVEL, message, filename, line);
    }
}

void Log::debug(const std::string& message, const char* filename, int line)
{
    if (Log::level <= Log::DEBUG_LEVEL) {
        Log::m_logger->log(Log::DEBUG_LEVEL, message.c_str(), filename, line);
    }
}

void Log::warn(const char* message, const char* filename, int line)
{
    if (Log::level <= Log::WARN_LEVEL) {
        Log::m_logger->log(Log::WARN_LEVEL, message, filename, line);
    }
}    

void Log::warn(const std::string& message, const char* filename, int line)
{
    if (Log::level <= Log::WARN_LEVEL) {
        Log::m_logger->log(Log::WARN_LEVEL, message.c_str(), filename, line);
    }
}

void Log::error(const char* message, const char* filename, int line)
{
    if (Log::level <= Log::ERROR_LEVEL) {
        Log::m_logger->log(Log::ERROR_LEVEL, message, filename, line);
    }
}    

void Log::error(const std::string& message, const char* filename, int line)
{
    if (Log::level <= Log::ERROR_LEVEL) {
        Log::m_logger->log(Log::ERROR_LEVEL, message.c_str(), filename, line);
    }
}

} // namespace faker_tsn
