#pragma once

#include <ostream>
#include <string>

class Logger
{
public:
    enum class Priority
    {
        Trace = 0,
        Debug = 1,
        Info = 2,
        Warn = 3,
        Error = 4
    };

private:
    std::ostream* m_output;
    Priority m_loggingLevel;
    bool m_isTimeStampEnabled;

public:
    static Logger& instance();

    void setOutput(std::ostream& newOutput);
    const std::ostream& output() const;

    Priority loggingLevel() const;
    void setLoggingLevel(Priority p);

    void setTimeStampEnabled(bool enable);
    bool isTimeStampEnabled();

    template <class Func>
    void logLazy(Priority p, Func&& evaluator)
    {
        if (static_cast<int>(p) < static_cast<int>(m_loggingLevel)) return;
        log(p, std::forward<Func>(evaluator)());
    }
    void log(Priority p, const std::string& message) const;
    void log(Priority p, const char* message) const;
private:
    Logger();

    void printTimeStamp() const;
    void printPriority(Priority p) const;
    void printMessage(const char* message) const;
};