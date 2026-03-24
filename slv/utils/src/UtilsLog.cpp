#include "UtilsLog.h"

void UtilsLog::write(LogLevel level,
                    const char* file,
                    int line,
                    const char* func,
                    const std::string& msg)
{
    std::lock_guard<std::mutex> lock(mtx);
    std::cout << levelToColor(level)
              << "[" << levelToString(level) << "] "
              << file << ":" << line
              << " (" << func << ") "
              << msg
              << "\033[0m"
              << std::endl;
}

const char* UtilsLog::levelToColor(LogLevel level)
{
    switch (level) {
        case LogLevel::DEBUG: return "\033[36m";
        case LogLevel::INFO:  return "\033[37m";
        case LogLevel::WARN:  return "\033[33m";
        case LogLevel::ERR:   return "\033[31m";
        default:              return "\033[0m";
    }
}

const char* UtilsLog::levelToString(LogLevel level)
{
    switch (level) {
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::INFO:  return "INFO";
        case LogLevel::WARN:  return "WARN";
        case LogLevel::ERR:   return "ERROR";
        default:              return "LOG";
    }
}
