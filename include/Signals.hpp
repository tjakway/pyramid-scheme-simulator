#pragma once

#include <memory>
#include <csignal>
#include <array>

#include "Simulation.hpp"

namespace pyramid_scheme_simulator {

std::array<const int, 1> SIGNALS_TO_HANDLE = {{ SIGINT }};

//exception thrown ***ONLY*** if there's an error *installing*
//the signal handler
//the actual signal handler will NOT throw an exception
NEW_EXCEPTION_TYPE(InstallSignalHandlerException);

void installSignalHandler(std::weak_ptr<Simulation>);

}
