#ifndef _UTILS_LOG_H
#define _UTILS_LOG_H

#include <iostream>
#include <string>
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

    static void write(LogLevel level, const char* file, int line, const char* func, const std::string& msg);
    static const char* levelToColor(LogLevel level);
    static const char* levelToString(LogLevel level);
};

// Macros
#define LOGD(fmt, ...) UtilsLog::log(UtilsLog::LogLevel::DEBUG, __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)
#define LOGI(fmt, ...) UtilsLog::log(UtilsLog::LogLevel::INFO,  __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)
#define LOGW(fmt, ...) UtilsLog::log(UtilsLog::LogLevel::WARN,  __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)
#define LOGE(fmt, ...) UtilsLog::log(UtilsLog::LogLevel::ERR,   __FILE__, __LINE__, __func__, fmt, ##__VA_ARGS__)

#endif // _UTILS_LOG_H
