#pragma once

#include <vector>

namespace pyramid_scheme_simulator {

enum TransformationType
{
    BOUGHT_PRODUCT=2,
    BECAME_DISTRIBUTOR=3
};

class Transformation
{
    //XXX
    //virtual void transform(graph, )
    virtual TransformationType getTransformationType() = 0;
};

//a step in the simulator is a set of transformations
//XXX: should each step also return a graph?
using Step = std::vector<Transformation>

} //pyramid_scheme_simulator
