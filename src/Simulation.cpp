#include <memory>

#include "Tick.hpp"
#include "Config.hpp"
#include "PopulationGraph.hpp"
#include "Simulation.hpp"

namespace pyramid_scheme_simulator {

Simulation::Simulation(Config* c) : config(std::shared_ptr<Config>(c)) 
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

ConversionHandler::Conversion* Simulation::lookupConversionRecord(
        ConversionHandler::RecordType recs,
        Unique possibleConvertId)
{
    auto res = std::find_if(recs.records.begin(), recs.records.end(),
            [=](const PopulationGraph::Pop x){
                return x->id == possibleConvertId;
            });

    if(res == recs.records.end())
    {
        return nullptr;
    }
    else
    {
        return (*res).get();
    }
}

}
