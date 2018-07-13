#pragma once

#include "NamespaceDefines.hpp"

#include "Types.hpp"
#include "Tick.hpp"

BEGIN_PYRAMID_NAMESPACE;

/**
 * straightforward data for tests
 */
class BasicGraphSettings
{
public:
    const Money price = 10;
    const Money startingMoney = 100;
    const Money buyIn = 1;

    //starting tick
    const SimulationTick when = 0;

    rd_ptr rd = std::make_shared<std::mt19937_64>();
};

END_PYRAMID_NAMESPACE;
