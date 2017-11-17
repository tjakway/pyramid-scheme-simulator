#pragma once

#include <set>
#include <vector>
#include <functional>

#include "CapitalHolderClassDecls.hpp"
#include "Unique.hpp"
#include "Tick.hpp"
#include "Config.hpp"
#include "ChanceContributor.hpp"

namespace pyramid_scheme_simulator {

//XXX: TODO: determine starting funds
class CapitalHolder : public Uniqueable
{
protected:
    Money money;

    double getMoneyToProductCostRatio();

    void setMoney(Money);

    virtual bool canPurchase(Money productCost, const CapitalHolder& from) {
        return money >= productCost;
    }

    CapitalHolder(Unique id, Money startingFunds) 
        : Uniqueable(id), money(startingFunds) {}

    CapitalHolder(Money startingFunds) 
        : CapitalHolder(emptyUnique, startingFunds) {}

public:
    virtual ~CapitalHolder() {}

    virtual ChanceContributor&
        getSalesChanceContribution();

    virtual ChanceContributor&
        getDistributorConversionChanceContribution();

    virtual bool isDistributor() { return false; }

    Money getMoney() const;

    bool operator==(const CapitalHolder&);
    bool operator!=(const CapitalHolder&);
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

class Distributor : public Consumer
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
    virtual bool canPurchase(Money cost, const CapitalHolder& from) override;

    bool isSubDistributor() { return recruitedBy.get() != nullptr; }

public:
    void addMoney(Money);

    virtual bool isDistributor() override { return true; }
};


} //pyramid_scheme_simulator
