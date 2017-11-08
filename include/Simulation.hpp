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
};

} //namespace pyramid_scheme_simulator
