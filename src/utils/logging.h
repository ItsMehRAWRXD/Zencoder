#pragma once

#include <string>
#include <iostream>

namespace Logging {
    
    enum class LogLevel {
        DEBUG,
        INFO,
        WARNING,
        ERROR
    };
    
    void log(LogLevel level, const std::string& message);
    void debug(const std::string& message);
    void info(const std::string& message);
    void warning(const std::string& message);
    void error(const std::string& message);
    
} // namespace Logging