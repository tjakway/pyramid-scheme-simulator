#include "Unique.hpp"
using namespace pyramid_scheme_simulator;

Unique UniqueGen::newUnique() 
{
    return Unique(rd->operator()());
}
