#pragma once

#include "NamespaceDefines.hpp"

#include "Types.hpp"
#include "Tick.hpp"
#include "HasTime.hpp"

BEGIN_PYRAMID_NAMESPACE;

/**
 * straightforward data for tests
 */
class BasicGraphSettings : public HasTime
{
public:
    const Money price = 10;
    const Money startingMoney = 100;
    const Money buyIn = 1;

    //starting tick
    const SimulationTick start = 0;
    SimulationTick now = start;

    virtual SimulationTick when() const override { return now; }

    rd_ptr rd = std::make_shared<std::mt19937_64>();
};

END_PYRAMID_NAMESPACE;
