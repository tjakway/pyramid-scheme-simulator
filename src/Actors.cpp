#include "Actors.hpp"
#include "Unique.hpp"
#include "Util.hpp"

#include <utility>
#include <memory>

namespace pyramid_scheme_simulator {


StaticConsumer::StaticConsumer(Unique u, Money m,
        std::unique_ptr<ChanceContributor> sChance,
        std::unique_ptr<ChanceContributor> cChance)
    : Consumer(u, m),
      salesChance(std::move(sChance)),
      conversionChance(std::move(cChance))
{ }


StaticConsumer::StaticConsumer(Unique u, Money m, 
        const double salesChance, const double conversionChance)
    : StaticConsumer(u, m,
            std::unique_ptr<ChanceContributor>
                (new StaticChanceContributor(salesChance)),
            std::unique_ptr<ChanceContributor>
                (new StaticChanceContributor(conversionChance)))
{}


StaticConsumer::StaticConsumer(StaticConsumer& c)
    : StaticConsumer(c.id, c.getMoney(),
            //copy pointer referands
            salesChance->clone(),
            conversionChance->clone())
{ }

ChanceContributor* StaticConsumer::getSalesChanceContribution()
{
    return salesChance.get();
}
ChanceContributor* StaticConsumer::getDistributorConversionChanceContribution()
{
    return conversionChance.get();
}

}
