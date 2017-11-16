#pragma once

#include <memory>

#include "Config.hpp"
#include "Graph.hpp"

namespace pyramid_scheme_simulator {

class Simulation
{
private:
    std::shared_ptr<Config> config;
    std::unique_ptr<PopulationGraph> graph;

    std::unique_ptr<PopulationGraph> buildGraph(std::shared_ptr<Config>);
    void tick();

public:
    Simulation(Config*);
};

} //namespace pyramid_scheme_simulator
