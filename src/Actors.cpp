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
            c.salesChance->clone().get(),
            c.conversionChance->clone().get())
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
    //can't use make_shared here because we're calling a protected constructor
    //of a class that StaticConsumer is a friend of
    return std::shared_ptr<StaticDistributor>(new StaticDistributor(*this, convertedBy));
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

void Company::deductMoney(Money amt)
{ 
    //record the payment but don't change the balance
    disbursements.emplace_back(amt);
}

//master constructor
StaticDistributor::StaticDistributor(Unique id,
        Money startingMoney,
        Inventory startingInventory,
        Inventory _desiredRestockAmount,
        Inventory _restockThreshold,
        std::unique_ptr<ChanceContributor>&& _salesChance,
        std::unique_ptr<ChanceContributor>&& _conversionChanceContribution)
    : Distributor(id, startingMoney, 
            std::shared_ptr<Distributor>(nullptr)),
        salesChance(std::move(_salesChance)),
        conversionChanceContribution(std::move(_conversionChanceContribution)),
        desiredRestockAmount(_desiredRestockAmount),
        restockThreshold(_restockThreshold)
{
    setInventory(startingInventory);

    PP_ASSERT(desiredRestockAmount > restockThreshold);
}


StaticDistributor::StaticDistributor(Unique id, 
        Money startingMoney, 
        Inventory startingInventory,
        Inventory _desiredRestockAmount,
        Inventory _restockThreshold,
        const double _salesChance,
        const double _conversionChanceContribution)
    : StaticDistributor(id, startingMoney, startingInventory,
        _desiredRestockAmount,
        _restockThreshold,
        make_unique<StaticChanceContributor>(_salesChance),
        make_unique<StaticChanceContributor>(_conversionChanceContribution))
{ }


//copy constructor
StaticDistributor::StaticDistributor(const StaticDistributor& other)
    : StaticDistributor(other.id, other.getMoney(), other.getInventory(),
            other.getDesiredRestockAmount(), other.getRestockThreshold(),
            *other.salesChance, *other.conversionChanceContribution)
{
    recruitedBy = other.recruitedBy;
}


StaticDistributor::StaticDistributor(Unique id, 
        Money startingMoney, 
        Inventory startingInventory,
        Inventory _desiredRestockAmount,
        Inventory _restockThreshold,
        const ChanceContributor& _salesChance,
        const ChanceContributor& _conversionChanceContribution)
    : StaticDistributor(id, 
            startingMoney, 
            startingInventory,
            _desiredRestockAmount,
            _restockThreshold,
            _salesChance.clone(),
            _conversionChanceContribution.clone())
{}

StaticDistributor::~StaticDistributor() {}

ChanceContributor& StaticDistributor::getSalesChanceContribution() const
{
    return *salesChance;
}

ChanceContributor&
        StaticDistributor::getDistributorConversionChanceContribution() const
{
    return *StaticDistributor::conversionChanceContribution;
}


Inventory StaticDistributor::getDesiredRestockAmount() const
{
    return desiredRestockAmount;
}

Inventory StaticDistributor::getRestockThreshold() const
{
    return restockThreshold;
}

std::shared_ptr<CapitalHolder> StaticDistributor::clone() const
{
    return std::make_shared<StaticDistributor>(*this);
}



}
