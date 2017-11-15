#include <memory>

#include "Tick.hpp"
#include "Config.hpp"
#include "Graph.hpp"
#include "Simulation.hpp"

namespace pyramid_scheme_simulator {

Simulation::Simulation(const Config* const c) : config(std::shared_ptr<const Config>(c)) 
{
    graph = buildGraph(config);
}

std::unique_ptr<PopulationGraph> buildGraph(const std::shared_ptr<Config> config)
{
    return std::unique_ptr<PopulationGraph>(new PopulationGraph(*config));
}



}
