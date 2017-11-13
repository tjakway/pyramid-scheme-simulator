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
