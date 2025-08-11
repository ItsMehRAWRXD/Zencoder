#include "logging.h"
#include <chrono>
#include <iomanip>
#include <sstream>

namespace Logging {

void log(LogLevel level, const std::string& message) {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto tm = *std::localtime(&time_t);
    
    std::stringstream ss;
    ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    
    std::string levelStr;
    switch (level) {
        case LogLevel::DEBUG:   levelStr = "DEBUG"; break;
        case LogLevel::INFO:    levelStr = "INFO"; break;
        case LogLevel::WARNING: levelStr = "WARNING"; break;
        case LogLevel::ERROR:   levelStr = "ERROR"; break;
    }
    
    std::cout << "[" << ss.str() << "] [" << levelStr << "] " << message << std::endl;
}

void debug(const std::string& message) {
    log(LogLevel::DEBUG, message);
}

void info(const std::string& message) {
    log(LogLevel::INFO, message);
}

void warning(const std::string& message) {
    log(LogLevel::WARNING, message);
}

void error(const std::string& message) {
    log(LogLevel::ERROR, message);
}

} // namespace Logging