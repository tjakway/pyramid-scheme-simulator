#pragma once

#include "Types.hpp"

namespace pyramid_scheme_simulator {

class ChanceContributor
{
public:
    virtual double getChance(rd_ptr) = 0;

    static bool sampleFrom(rd_ptr, ChanceContributor&, ChanceContributor&);

    virtual std::unique_ptr<ChanceContributor> clone() = 0;
};

/**
 * a ChanceContributor that doesn't select from any distribution
 * but just uses the chance passed to its constructor
 */
class StaticChanceContributor : public ChanceContributor
{
    double chance;
public:
    StaticChanceContributor(double staticChance);

    virtual double getChance(rd_ptr) override;
    virtual std::unique_ptr<ChanceContributor> clone() override;
};

}
