#ifndef LOGGING_H
#define LOGGING_H

#include <string>

namespace Logging {

enum class LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

void setLogLevel(LogLevel level);
void log(LogLevel level, const std::string& message);

} // namespace Logging

#endif // LOGGING_H