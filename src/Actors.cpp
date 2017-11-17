#include "Actors.hpp"
#include "Unique.hpp"
#include "Util.hpp"

#include <utility>
#include <memory>
#include <limits>

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

/**
 * the company would never buy from one of its own distributors that it sold
 * its product to
 */
bool Company::canPurchase(Money cost, const CapitalHolder& from)
{
    return false;
}

/**
 * the company will never run out of its products
 */
unsigned int Company::getInventory()
{
    return std::numeric_limits<unsigned int>::max();
}

}
