#pragma once

#include <memory>
#include <algorithm>

#include "Config.hpp"
#include "PopulationGraph.hpp"
#include "TransactionObjects.hpp"
#include "TransactionRecords.hpp"

namespace pyramid_scheme_simulator {

class Simulation
{
private:
    std::shared_ptr<Config> config;
    std::unique_ptr<PopulationGraph> populationGraph;

    ConversionHandler conversionHandler;

    SimulationTick now = 0;
    SimulationTick when() const { return now; }

    std::unique_ptr<PopulationGraph> buildGraph(std::shared_ptr<Config>);
    void tick();
    
protected:
    static ConversionHandler::Conversion* lookupConversionRecord(
            ConversionHandler::RecordType&,
            Unique);

    PopulationGraph::vertices_size_type 
        processConversions(ConversionHandler::RecordType&);

public:
    Simulation(Config*);
};

} //namespace pyramid_scheme_simulator
