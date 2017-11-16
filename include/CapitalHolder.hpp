#pragma once

#include <set>
#include <vector>
#include <functional>

#include "Unique.hpp"
#include "Tick.hpp"
#include "Config.hpp"
#include "ChanceContributor.hpp"

namespace pyramid_scheme_simulator {

class CapitalHolder;
class Consumer;
class Distributor;

//XXX: TODO: determine starting funds
class CapitalHolder : public Uniqueable
{
protected:
    Money money;

    double getMoneyToProductCostRatio();

    void setMoney(Money);
    Money getMoney() const;

    virtual bool canPurchase(Money productCost, const CapitalHolder& from) {
        return money >= productCost;
    }

    CapitalHolder(Unique id, Money startingFunds) 
        : Uniqueable(id), money(startingFunds) {}

    CapitalHolder(Money startingFunds) 
        : CapitalHolder(emptyUnique, startingFunds) {}

    bool operator==(const CapitalHolder&);
    bool operator!=(const CapitalHolder&);

public:
    virtual ~CapitalHolder() {}

    virtual ChanceContributor*
        getSalesChanceContribution();

    virtual ChanceContributor*
        getDistributorConversionChanceContributor();

    virtual bool isDistributor() { return false; }
};

//TODO
class Consumer : public CapitalHolder
{
public:
    Consumer(Unique id, Money startingFunds): CapitalHolder(id, startingFunds) {}

    void deductMoney(Money);

    virtual std::unique_ptr<Distributor> 
        becomeDistributor(Config::SimulationOptions::DistributorOptions&,
                Distributor* convertedBy);
};

class Distributor : protected Consumer
{
private:
    /**
     * who this distributor was recruited by
     * nullptr if they directly bought in from the company
     */
    std::shared_ptr<Distributor> recruitedBy;
    //TODO: add downstreamPercent field
protected:
    /**
     * chance of making a sale to the other node
     * TODO: probably make this a static method that takes both the Consumer and Distributor
     * objects as parameters since the chance of a sale depends on factors from both
     */
    virtual double getSalesChance(const CapitalHolder& x) = 0;
    virtual bool canPurchase(Money cost, const CapitalHolder& from);

    bool isSubDistributor() { return recruitedBy.get() != nullptr; }

public:
    void addMoney(Money);

    virtual bool isDistributor() override { return true; }
};


} //pyramid_scheme_simulator
