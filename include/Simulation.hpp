#pragma once

#include <memory>

#include "Config.hpp"
#include "Graph.hpp"

namespace pyramid_scheme_simulator {

class Simulation
{
    const std::shared_ptr<const Config> config;

    std::unique_ptr<PopulationGraph> buildGraph(const std::shared_ptr<Config>);

public:
    Simulation(const Config* const c) : config(std::shared_ptr<const Config>(c)) {}
};

} //namespace pyramid_scheme_simulator
