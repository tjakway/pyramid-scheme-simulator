#include "LoggingBackend.hpp"

#include "Util/Strcat.hpp"
#include <sstream>

namespace pyramid_scheme_simulator {

void LoggingBackend::interrupt() 
{
    interruptedFlag.store(true);
}

bool LoggingBackend::interrupted() const
{
    return interruptedFlag.load();
}

LoggingBackend::LoggingBackend()
{
    interruptedFlag.store(false);
}

/**
 * log level is set to "info" by default--
 * we're logging the graph structure, it's not *that* important
 */
const spdlog::level::level_enum SpdLoggingBackend::defaultLogLevel
    = spdlog::level::info;


//just flush if we're interrupted
//destructors take care of anything important anyway,
//probably don't even need to explicitly do this
void SpdLoggingBackend::onInterrupt()
{
    logger->flush();
}

void SpdLoggingBackend::writeString(const std::string& toWrite)
{
    //see http://www.cplusplus.com/reference/string/string/getline/
    std::stringstream ss(toWrite);
    std::string line;

    while(std::getline(ss, line))
    {
        //check the interrupt flag
        if(interrupted())
        {
            onInterrupt();
            return;
        }
        else
        {
            writeLine(line);
        }
    }
}

//the smallest non-interruptible amount of work
void SpdLoggingBackend::writeLine(const std::string& line)
{
    logger->info(line);
}

SpdLoggingBackend::SpdLoggingBackend(
        std::shared_ptr<spdlog::logger> _logger,
        spdlog::level::level_enum _lvl)
    : logger(_logger)
{
    logger->set_level(_lvl);
}

SpdLoggingBackend::SpdLoggingBackend(std::shared_ptr<spdlog::logger> _logger)
    : SpdLoggingBackend(_logger, defaultLogLevel)
{}

const std::string ConsoleLoggingBackend::loggerName  = "graph_logger";

ConsoleLoggingBackend::ConsoleLoggingBackend()
    : SpdLoggingBackend(spdlog::stdout_logger_mt(loggerName))
{}

const std::string FileLoggingBackend::loggerName = "graph_file_logger";

FileLoggingBackend::FileLoggingBackend(const std::string& filename)
    : SpdLoggingBackend(spdlog::basic_logger_mt(loggerName, filename))
{}


/**
 * the important part
 * actually writing the graph as a string
 */
void SpdLoggingBackend::exportGraph(std::shared_ptr<PopulationGraph> graph,
        const SimulationTick when)
{
    writeString(
        STRCAT("PopulationGraph, tick #", when, 
                std::endl, 
                graph->toString(), 
                std::endl));

}

}
