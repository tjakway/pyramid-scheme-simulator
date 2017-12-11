#pragma once

#include "PopulationGraph.hpp"
#include "CapitalHolder.hpp"

#include <utility>
#include <vector>

namespace pyramid_scheme_simulator {

class MockPopulationGraph : public PopulationGraph
{
public:
    MockPopulationGraph(std::vector<std::pair<Pop, Pop>> tuples)
        : PopulationGraph(PopulationGraph::graphFromTuples(tuples))
    {}

    MockPopulationGraph(Config& config)
        : PopulationGraph(config)
    {}

    PopulationGraph::BGLPopulationGraph* getGraphPtr() { return &graph; }

    using MockUndirectedEdge = PopulationGraph::UndirectedEdge;
};

}
