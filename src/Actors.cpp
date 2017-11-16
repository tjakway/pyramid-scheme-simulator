#include "Actors.hpp"

namespace pyramid_scheme_simulator {

StaticConsumer(Unique, Money, 
        const double salesChance, const double conversionChance)
    : Consumer(u, m),
        salesChance(new StaticChanceContributor(salesChance)),
        conversionChance(new StaticChanceContributor(conversionChance))

virtual ChanceContributor* getSalesChanceContribution() override
{
    return salesChance.get();
}
virtual ChanceContributor* getDistributorConversionChanceContributor() override
{
    return conversionChance.get();
}

}
