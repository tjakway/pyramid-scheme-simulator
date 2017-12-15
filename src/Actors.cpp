#include "Actors.hpp"
#include "Util/Unique.hpp"
#include "Util/Util.hpp"

#include <utility>
#include <memory>
#include <limits>

namespace pyramid_scheme_simulator {


StaticConsumer::StaticConsumer(Unique u, Money m,
        std::unique_ptr<ChanceContributor>&& sChance,
        std::unique_ptr<ChanceContributor>&& cChance)
    : Consumer(u, m),
    salesChance(std::move(sChance)),
    conversionChance(std::move(cChance))
{}


StaticConsumer::StaticConsumer(Unique u, Money m,
        ChanceContributor* sChance,
        ChanceContributor* cChance)
    : StaticConsumer(u, m, sChance->clone(), cChance->clone())
{}

StaticConsumer::StaticConsumer(Unique u, Money m,
        std::unique_ptr<ChanceContributor>& s,
        std::unique_ptr<ChanceContributor>& c)
    : StaticConsumer(u, m, s.get(), c.get())
{}

StaticConsumer::StaticConsumer(Unique u, Money m, 
        const double salesChance, const double conversionChance)
    : StaticConsumer(u, m,
                make_unique<StaticChanceContributor>(salesChance),
                make_unique<StaticChanceContributor>(
                    StaticChanceContributor(conversionChance)))
{}


//copy constructor
StaticConsumer::StaticConsumer(const StaticConsumer& c)
    : StaticConsumer(c.id, c.getMoney(),
            //copy pointer referands
            salesChance->clone().get(),
            conversionChance->clone().get())
{}


std::shared_ptr<CapitalHolder> StaticConsumer::clone() const 
{
    //call copy constructor
    return std::make_shared<StaticConsumer>(*this);
}

ChanceContributor& StaticConsumer::getSalesChanceContribution() const
{
    return *salesChance.get();
}
ChanceContributor& StaticConsumer::getDistributorConversionChanceContribution() const
{
    return *conversionChance.get();
}

std::shared_ptr<Distributor> 
    StaticConsumer::becomeDistributor(std::shared_ptr<Distributor> convertedBy)
{
    return std::make_shared<StaticDistributor>(*this, convertedBy);
}

const unsigned int Company::inventory = std::numeric_limits<unsigned int>::max();

/**
 * the company would never buy from one of its own distributors that it sold
 * its product to
 */
bool Company::canPurchase(Money /*cost*/, const CapitalHolder& /*from*/)
{
    return false;
}

/**
 * the company will never run out of its products
 */
unsigned int Company::getInventory() const
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
        Inventory startingInventory,
        ChanceContributor* _salesChance)
    : StaticDistributor(id, startingMoney,
            startingInventory,
            _salesChance->clone())
{ }

StaticDistributor::StaticDistributor(Unique id, 
        Money startingMoney, 
        Inventory startingInventory)
    : StaticDistributor(id, startingMoney, startingInventory, nullptr)
{ }

StaticDistributor::StaticDistributor(Unique id,
        Money startingMoney,
        Inventory startingInventory,
        std::unique_ptr<ChanceContributor>&& _salesChance)
    : Distributor(id, startingMoney, std::shared_ptr<Distributor>(nullptr)),
        salesChance(std::move(_salesChance))
{
    setInventory(startingInventory);
}


StaticDistributor::StaticDistributor(Unique id, 
        Money startingMoney, 
        Inventory startingInventory,
        const double _salesChance)
    : StaticDistributor(id, startingMoney, startingInventory,
        make_unique<StaticChanceContributor>(_salesChance))
{ }


//copy constructor
StaticDistributor::StaticDistributor(const StaticDistributor& other)
    : StaticDistributor(other.id, other.getMoney(), other.getInventory(),
            other.salesChance.get())
{
    recruitedBy = other.recruitedBy;
}


StaticDistributor::StaticDistributor(Unique id, 
        Money startingMoney, 
        Inventory startingInventory,
        std::unique_ptr<ChanceContributor>& _salesChance)
    : StaticDistributor(id, startingMoney, startingInventory,
        _salesChance.get())
{}

StaticDistributor::~StaticDistributor() {}

ChanceContributor& StaticDistributor::getSalesChanceContribution() const
{
    return *salesChance;
}

const std::unique_ptr<ChanceContributor> StaticDistributor::conversionChance =
    make_unique<StaticChanceContributor>(0.0);

ChanceContributor&
        StaticDistributor::getDistributorConversionChanceContribution() const
{
    return *StaticDistributor::conversionChance;
}

std::shared_ptr<CapitalHolder> StaticDistributor::clone() const
{
    return std::make_shared<StaticDistributor>(*this);
}



}
