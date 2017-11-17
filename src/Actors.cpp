#include "Actors.hpp"
#include "Unique.hpp"
#include "Util.hpp"

#include <utility>
#include <memory>

namespace pyramid_scheme_simulator {


StaticConsumer::StaticConsumer(Unique u, Money m,
        ChanceContributor* sChance,
        ChanceContributor* cChance)
    : Consumer(u, m),
    salesChance(std::move(sChance->clone())),
    conversionChance(std::move(cChance->clone()))
{ }

StaticConsumer::StaticConsumer(Unique u, Money m,
        std::unique_ptr<ChanceContributor>& s,
        std::unique_ptr<ChanceContributor>& c)
    : StaticConsumer(u, m, s.get(), c.get())
{}

StaticConsumer::StaticConsumer(Unique u, Money m, 
        const double salesChance, const double conversionChance)
    : StaticConsumer(u, m,
                new StaticChanceContributor(salesChance),
                new StaticChanceContributor(conversionChance))
{}


StaticConsumer::StaticConsumer(StaticConsumer& c)
    : StaticConsumer(c.id, c.getMoney(),
            //copy pointer referands
            salesChance->clone().get(),
            conversionChance->clone().get())
{ }

ChanceContributor& StaticConsumer::getSalesChanceContribution()
{
    return *salesChance.get();
}
ChanceContributor& StaticConsumer::getDistributorConversionChanceContribution()
{
    return *conversionChance.get();
}

}
