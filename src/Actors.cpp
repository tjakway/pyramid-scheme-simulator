#include "Actors.hpp"
#include "Unique.hpp"

#include <memory>

namespace pyramid_scheme_simulator {


StaticConsumer::StaticConsumer(Unique u, Money m,
        const std::unique_ptr<ChanceContributor> sChance,
        const std::unique_ptr<ChanceContributor> cChance)
    : Consumer(u, m),
      salesChance(sChance),
      conversionChance(cChance)
{ }


StaticConsumer::StaticConsumer(Unique u, Money m, 
        const double salesChance, const double conversionChance)
    : StaticConsumer(u, m,
            new StaticChanceContributor(salesChance),
            new StaticChanceContributor(conversionChance))
{}

virtual ChanceContributor* getSalesChanceContribution() override
{
    return salesChance.get();
}
virtual ChanceContributor* getDistributorConversionChanceContributor() override
{
    return conversionChance.get();
}

}
