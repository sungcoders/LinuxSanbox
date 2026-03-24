#ifndef _UTILS_LOG_H
#define _UTILS_LOG_H

#include <iostream>
#include <string>
#include <cerrno>
#include <cstdlib>
#include <mutex>
#include <fmt/core.h>
#include <fmt/format.h>

class UtilsLog
{
public:
    enum class LogLevel {
        DEBUG,
        INFO,
        WARN,
        ERR
    };

    template<typename... Args>
    static void log(LogLevel level,
                    const char* file,
                    int line,
                    const char* func,
                    fmt::format_string<Args...> fmt,
                    Args&&... args)
    {
        write(level, file, line, func, fmt::format(fmt, std::forward<Args>(args)...));
    }

private:
    static inline std::mutex mtx;
    static void write(LogLevel level,
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

    static const char* levelToColor(LogLevel level)
    {
        switch (level) {
            case LogLevel::DEBUG: return "\033[36m";
            case LogLevel::INFO:  return "\033[37m";
            case LogLevel::WARN:  return "\033[33m";
            case LogLevel::ERR:   return "\033[31m";
            default:              return "\033[0m";
        }
    }

    static const char* levelToString(LogLevel level)
    {
        switch (level) {
            case LogLevel::DEBUG: return "DEBUG";
            case LogLevel::INFO:  return "INFO";
            case LogLevel::WARN:  return "WARN";
            case LogLevel::ERR:   return "ERROR";
            default:              return "LOG";
        }
    }

#define LOGD(fmt, ...) UtilsLog::log(UtilsLog::LogLevel::DEBUG, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)
#define LOGI(fmt, ...) UtilsLog::log(UtilsLog::LogLevel::INFO,  __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)
#define LOGW(fmt, ...) UtilsLog::log(UtilsLog::LogLevel::WARN,  __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)
#define LOGE(fmt, ...) UtilsLog::log(UtilsLog::LogLevel::ERR,   __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)

};

#endif // LOG_H
