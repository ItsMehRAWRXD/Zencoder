#include "logging.h"
#include <iostream>
#include <chrono>
#include <iomanip>

namespace Logging {

static LogLevel currentLevel = LogLevel::INFO;

void setLogLevel(LogLevel level) {
    currentLevel = level;
}

void log(LogLevel level, const std::string& message) {
    if (level < currentLevel) {
        return;
    }
    
    // Get current time
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    
    // Format log message
    std::cout << "[" << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H:%M:%S") << "] ";
    
    switch (level) {
        case LogLevel::DEBUG:
            std::cout << "[DEBUG] ";
            break;
        case LogLevel::INFO:
            std::cout << "[INFO] ";
            break;
        case LogLevel::WARNING:
            std::cout << "[WARN] ";
            break;
        case LogLevel::ERROR:
            std::cout << "[ERROR] ";
            break;
    }
    
    std::cout << message << std::endl;
}

} // namespace Logging