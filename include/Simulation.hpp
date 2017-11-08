#pragma once

#include <memory>

#include "Config.hpp"
#include "Graph.hpp"

namespace pyramid_scheme_simulator {

class Simulation
{
    const std::shared_ptr<Config> config;

    std::unique_ptr<PopulationGraph> buildGraph(const std::shared_ptr<Config>);

public:
    Simulation(Config c) : config(c) {}

    /**
     * represents 1 step of the simulation
     */
    using tick = unsigned int;
};

} //namespace pyramid_scheme_simulator
