#include "Signals.hpp"

#include "Simulation.hpp"
#include "Config.hpp"
#include <memory>
#include <exception>
#include <csignal>

static std::weak_ptr<pyramid_scheme_simulator::Simulation> simPtr;

extern "C"{
    static void signalHandler(int signal)
    {
        using namespace pyramid_scheme_simulator;
        //make sure we want to handle this signal
        if(std::find(SIGNALS_TO_HANDLE.begin(), SIGNALS_TO_HANDLE.end(),
                    signal) 
                == SIGNALS_TO_HANDLE.end())
        {
            if(Config::logger.get() != nullptr)
            { Config::logger->info("Received signal {}, ignoring", signal); }
            return;
        }
        else
        {
            if(Config::logger.get() != nullptr)
            { Config::logger->info("Received signal {}", signal); }
        }


        //try and interrupt backends if possible
        if(auto sim = simPtr.lock())
        {
            sim->interrupt();
        }
        else
        { }

        std::exit(1);
    }
}

namespace pyramid_scheme_simulator {

#ifdef ENABLE_SIGNAL_HANDLER
void installSignalHandler(std::weak_ptr<Simulation> _simPtr)
{
    try {
        simPtr = _simPtr;
        for(const int whichSignal : SIGNALS_TO_HANDLE)
        {
            std::signal(whichSignal, signalHandler);
            Config::logger->debug("Installed signal handler for signal code {}", whichSignal);
        }
    }
    catch(const std::exception& e)
    {
        throw InstallSignalHandlerException(
                STRCAT("Error installing signal handler caused by ",
                "exception: ", e.what()));
    }
}
#else
    //no-op
void installSignalHandler(std::weak_ptr<Simulation>)
{}
#endif
}
