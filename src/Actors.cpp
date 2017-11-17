#include "Actors.hpp"
#include "Unique.hpp"
#include "Util.hpp"

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
            make_unique<StaticChanceContributor>(salesChance),
            make_unique<StaticChanceContributor>(conversionChance))
{}

ChanceContributor* StaticConsumer::getSalesChanceContribution()
{
    return salesChance.get();
}
ChanceContributor* StaticConsumer::getDistributorConversionChanceContribution()
{
    return conversionChance.get();
}

}
