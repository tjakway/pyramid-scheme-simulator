#pragma once

#include "Types.hpp"

namespace pyramid_scheme_simulator {

class ChanceContributor
{
public:
    virtual double getChance(rd_ptr) = 0;

    static bool sampleFrom(rd_ptr, ChanceContributor&, ChanceContributor&);
};

/**
 * a ChanceContributor that doesn't select from any distribution
 * but just uses the chance passed to its constructor
 */
class StaticChanceContributor : ChanceContributor
{
public:
    StaticChanceContributor(double staticChance);

    virtual double getChance(rd_ptr);
};

}
