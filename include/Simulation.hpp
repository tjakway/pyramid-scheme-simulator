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
    std::unique_ptr<PopulationGraph> graph;

    std::unique_ptr<PopulationGraph> buildGraph(std::shared_ptr<Config>);
    void tick();
    
protected:
    ConversionHandler::Conversion* lookupConversionRecord(ConversionHandler::RecordType recs,
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

public:
    Simulation(Config*);
};

} //namespace pyramid_scheme_simulator
