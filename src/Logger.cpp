#include "Logger.h"

Logger& Logger::getInstance()
{
    static Logger instance;
    return instance;
}

void Logger::log(LogLevel level, const char *message)
{
    std::lock_guard<std::mutex> guard(logMutex);
    if (level <= currentLevel)
    {
        switch (level)
        {
        case LOG_LEVEL_ERROR:
            Serial.print("ERROR: ");
            break;
        case LOG_LEVEL_WARN:
            Serial.print("WARN: ");
            break;
        case LOG_LEVEL_INFO:
            Serial.print("INFO: ");
            break;
        case LOG_LEVEL_DEBUG:
            Serial.print("DEBUG: ");
            break;
        }
        Serial.println(message);
    }
}

void Logger::setLogLevel(LogLevel level)
{
    std::lock_guard<std::mutex> guard(logMutex);
    currentLevel = level;
}

Logger::Logger() : currentLevel(LOG_LEVEL_INFO)
{
    Serial.begin(115200);
}