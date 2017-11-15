#pragma once

#include <memory>

#include "Config.hpp"
#include "Graph.hpp"

namespace pyramid_scheme_simulator {

class Simulation
{
private:
    const std::shared_ptr<Config> config;
    std::unique_ptr<PopulationGraph> graph;

    std::unique_ptr<PopulationGraph> buildGraph(const std::shared_ptr<Config>);
    void tick();

public:
    Simulation(const Config* const c);
};

} //namespace pyramid_scheme_simulator
