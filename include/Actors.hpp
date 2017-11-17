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

    StaticConsumer(Unique, Money,
            const std::unique_ptr<ChanceContributor>,
            const std::unique_ptr<ChanceContributor>);

    virtual ChanceContributor*
        getSalesChanceContribution() override;

    virtual ChanceContributor*
        getDistributorConversionChanceContribution() override;
};

}
