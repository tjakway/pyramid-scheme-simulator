#pragma once

#include "Unique.hpp"
#include "ChanceContributor.hpp"
#include "CapitalHolder.hpp"

#include <memory>

namespace pyramid_scheme_simulator {

class StaticConsumer : public Consumer
{
    const std::unique_ptr<ChanceContributor> salesChance;
    const std::unique_ptr<ChanceContributor> conversionChance;
public:
    StaticConsumer(Unique, Money, 
            const double salesChance, const double conversionChance);

    virtual ChanceContributor*
        getSalesChanceContribution() override;

    virtual ChanceContributor*
        getDistributorConversionChanceContributor() override;
};

class ExpConsumer : public Consumer {
    /**
     * constant factor from 0-1 reflecting how much this consumer likes
     * the product in question
     */
    double innateLikelihood;
public:

};

}
