#pragma once

#include "CapitalHolder.hpp"
#include "Config.hpp"
#include "Unique.hpp"

namespace pyramid_scheme_simulator {

//class to hold all global state
class Global
{
    Config config;
    UniqueGen uniqueGen;

    Company company;
    //the graph
};

}
