#include <memory>

#include "Tick.hpp"
#include "Config.hpp"
#include "PopulationGraph.hpp"
#include "Simulation.hpp"

namespace pyramid_scheme_simulator {

Simulation::Simulation(Config* c, std::vector<std::unique_ptr<Backend>>&& _backends) 
    : config(std::shared_ptr<Config>(c)),
    backends(std::move(_backends))
{
    graph = buildGraph(config);
}

std::unique_ptr<PopulationGraph> buildGraph(std::shared_ptr<Config> config)
{
    return std::unique_ptr<PopulationGraph>(new PopulationGraph(*config));
}


void Simulation::tick()
{
    //for each edge,
    //add the SalesResult if success or if we need it for logging
}

}
