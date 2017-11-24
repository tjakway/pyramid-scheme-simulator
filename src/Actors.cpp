#include "Actors.hpp"
#include "Util/Unique.hpp"
#include "Util/Util.hpp"

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
{}

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
{}

ChanceContributor& StaticConsumer::getSalesChanceContribution()
{
    return *salesChance.get();
}
ChanceContributor& StaticConsumer::getDistributorConversionChanceContribution()
{
    return *conversionChance.get();
}


const unsigned int Company::inventory = std::numeric_limits<unsigned int>::max();

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

/**
 * no-op because the company never buys anything
 */
void Company::deductMoney(Money)
{ return; }

StaticDistributor::StaticDistributor(Unique id, 
        Money startingMoney, 
        Inventory startingInventory)
    : StaticDistributor(id, startingMoney, startingInventory, nullptr)
{
    setInventory(startingInventory);
}

StaticDistributor::StaticDistributor(Unique id, 
        Money startingMoney, 
        Inventory startingInventory,
        ChanceContributor* _salesChance)
    : Distributor(id, startingMoney, std::shared_ptr<Distributor>(nullptr)),
      salesChance(_salesChance->clone())
    {}



StaticDistributor::StaticDistributor(Unique id, 
        Money startingMoney, 
        Inventory startingInventory,
        const double _salesChance)
    : StaticDistributor(id, startingMoney, startingInventory,
        new StaticChanceContributor(_salesChance))
{ }



StaticDistributor::StaticDistributor(Unique id, 
        Money startingMoney, 
        Inventory startingInventory,
        std::unique_ptr<ChanceContributor>& _salesChance)
    : StaticDistributor(id, startingMoney, startingInventory,
        _salesChance.get())
{}

ChanceContributor& StaticDistributor::getSalesChanceContribution()
{
    return *salesChance.get();
}

const std::unique_ptr<ChanceContributor> StaticDistributor::conversionChance =
    make_unique<StaticChanceContributor>(0.0);

ChanceContributor&
        StaticDistributor::getDistributorConversionChanceContribution()
{
    return *StaticDistributor::conversionChance;
}

}
