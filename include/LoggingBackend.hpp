#pragma once

#include "Simulation.hpp"

#include <atomic>
#include <string>

#include <spdlog/spdlog.h>

namespace pyramid_scheme_simulator {


class LoggingBackend : public Simulation::Backend
{
    //implements interrupt() via an atomic flag that
    //derived classes must cooperate with
    //(i.e. they have to check it and stop if it's true)
    std::atomic_bool interruptedFlag;

public:
    virtual void interrupt() noexcept override;
    virtual bool interrupted() const noexcept override;

protected:
    virtual void onInterrupt() const noexcept = 0;

    LoggingBackend();
    virtual ~LoggingBackend() {}

};

class SpdLoggingBackend : public LoggingBackend
{
private:
    void writeString(const std::string&);

protected:
    static const spdlog::level::level_enum defaultLogLevel;
    std::shared_ptr<spdlog::logger> logger;

    virtual void onInterrupt() const noexcept override;
    virtual void writeLine(const std::string&);

public:
    virtual void exportData(const std::shared_ptr<Data>) override;

    SpdLoggingBackend(std::shared_ptr<spdlog::logger>,
            spdlog::level::level_enum);

    SpdLoggingBackend(std::shared_ptr<spdlog::logger>);

    virtual ~SpdLoggingBackend() {}
};

class ConsoleLoggingBackend : public SpdLoggingBackend
{
    static const std::string loggerName;

public:
    ConsoleLoggingBackend();
    virtual ~ConsoleLoggingBackend() {}
};

class FileLoggingBackend : public SpdLoggingBackend
{
    static const std::string loggerName;

public:
    FileLoggingBackend(const std::string& filename);
    virtual ~FileLoggingBackend() {}
};

}
