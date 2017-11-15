#pragma once

#include <vector>
#include <unordered_set>
#include <memory>

#include <boost/graph/graph_traits.hpp>
#include <boost/graph/adjacency_list.hpp>

#include "Config.hpp"
#include "CapitalHolder.hpp"

namespace pyramid_scheme_simulator {

//vertex type
using Pop = std::shared_ptr<CapitalHolder>;
using PopEdge = std::pair<Pop, Pop>;

using PopulationGraph = 
    boost::adjacency_list<
        boost::vecS,
        boost::vecS, 
        boost::undirectedS,
        Pop>;

using PopDescriptor = 
    boost::graph_traits<PopulationGraph>::vertex_descriptor;

class PopulationGraphGenerator
{
private:
    //TODO: *maybe* make this a GraphGenerationOptions field
    unsigned int maxTries;

    bool checkGraph(const PopulationGraph&);

    /**
     * std::vector<std::unordered_set<Pop>> maps the vector index to the 
     * subgraph containing those vertices
     * so:
     *
     * auto ret = getDisconnectedSubgraphs(g);
     * ret.size() // <-- number of disconnected subgraphs
     *            //     (1 if the whole graph is connected)
     * ret[0]     // <-- subgraph #1
     * ret[1]     // <-- subgraph #2
     *            // and so on
     */
    std::vector<std::unordered_set<Pop>> getDisconnectedSubgraphs(const PopulationGraph&);

    PopulationGraph* buildGraph(rd_ptr, Config&);

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
