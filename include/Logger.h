#pragma once

#include <Arduino.h>
#include <mutex>

class Logger {
public:
    enum LogLevel {
        LOG_LEVEL_ERROR,
        LOG_LEVEL_WARN,
        LOG_LEVEL_INFO,
        LOG_LEVEL_DEBUG
    };

    static Logger& getInstance();
    void log(LogLevel level, const char* message);
    void setLogLevel(LogLevel level);

private:
    LogLevel currentLevel;
    std::mutex logMutex;

    Logger();

    // Delete copy constructor and assignment operator to prevent copies of the singleton instance
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
};