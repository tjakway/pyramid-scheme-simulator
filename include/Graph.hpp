#pragma once

#include <vector>
#include <memory>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>

#include "CapitalHolder.hpp"

namespace pyramid_scheme_simulator {

using PopulationGraph = 
    boost::adjacency_list<
        boost::vecS,
        boost::vecS, 
        boost::undirectedS,
        std::shared_ptr<CapitalHolder>>;

using PopDescriptor = 
    boost::graph_traits<PopulationGraph>::vertex_descriptor;

class PopulationGraphGenerator
{
private:
    //TODO: *maybe* make this a GraphGenerationOptions field
    unsigned int maxTries;

    bool checkGraph(const PopulationGraph&);

    

    PopulationGraph* buildGraph(rd_ptr, const Config::GraphGenerationOptions&);

    /**
     * TODO:
     * need this step to create Distributors during graph generation
     *  -need to check that all disconnected subgraphs have at least 1 distributor
     */
    PopulationGraph applyOnlyInitialOnboardingTransformation(PopulationGraph&);

public:
    PopulationGraphGenerator(const Config::GraphGenerationOptions&);
};

enum TransformationType
{
    BOUGHT_PRODUCT=2,
    BECAME_DISTRIBUTOR=3
};

class Transformation
{
    //XXX
    virtual void transform(PopulationGraph* graph, 
            PopDescriptor to,
            PopDescriptor from) = 0;
    virtual TransformationType getTransformationType() = 0;
};

//a step in the simulator is a set of transformations
//XXX: should each step also return a graph?
using Step = std::vector<Transformation>;

} //pyramid_scheme_simulator
