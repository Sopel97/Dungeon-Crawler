#include "Logger.h"

#include <iostream>
#include <chrono>
#include <ctime>
#include <iomanip>

Logger& Logger::instance()
{
    static Logger logger;
    return logger;
}

void Logger::setOutput(std::ostream& newOutput)
{
    m_output = &newOutput;
}
const std::ostream& Logger::output() const
{
    return *m_output;
}

Logger::Priority Logger::loggingLevel() const
{
    return m_loggingLevel;
}
void Logger::setLoggingLevel(Priority p)
{
    m_loggingLevel = p;
}

void Logger::setTimeStampEnabled(bool enable)
{
    m_isTimeStampEnabled = enable;
}
bool Logger::isTimeStampEnabled()
{
    return m_isTimeStampEnabled;
}

void Logger::log(Priority p, const std::string& message) const
{
    log(p, message.c_str());
}
void Logger::log(Priority p, const char* message) const
{
    if (static_cast<int>(p) < static_cast<int>(m_loggingLevel)) return;
    if (m_isTimeStampEnabled) printTimeStamp();
    printPriority(p);
    printMessage(message);
    (*m_output) << '\n';
}

Logger::Logger() :
    m_output(&std::cerr),
    m_loggingLevel(Priority::Debug),
    m_isTimeStampEnabled(true)
{

}

void Logger::printTimeStamp() const
{
    std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    (*m_output) << '[' << std::put_time(std::localtime(&now_c), "%F %T") << "] ";
}

void Logger::printPriority(Priority p) const
{
    static const std::string_view s[] = {
        "TRACE",
        "DEBUG",
        "INFO",
        "WARN",
        "ERROR"
    };

    (*m_output) << '[' << s[static_cast<int>(p)].data() << "] ";
}
void Logger::printMessage(const char* message) const
{
    (*m_output) << message;
}