#pragma once

#include <set>
#include <vector>
#include <functional>

#include "Unique.hpp"
#include "Tick.hpp"

namespace pyramid_scheme_simulator {

class CapitalHolder;
class Consumer;
class Distributor;

class CapitalHolder : public Uniqueable
{
protected:
    Money money;
    Money productCost;

    double getMoneyToProductCostRatio();

    void setMoney(Money);
    virtual bool canPurchase(const Distributor& from) {
        return money >= productCost;
    }

    virtual bool willPurchase(const Distributor& from) = 0;

    CapitalHolder(Money cost) : Uniqueable(), productCost(cost) {}

    bool operator==(const CapitalHolder&);
    bool operator!=(const CapitalHolder&);
};

//TODO
class Consumer : protected CapitalHolder
{
protected:
    Consumer(Money cost): CapitalHolder(cost) {}
public:
    void onBuy(const Distributor& from, SimulationTick when);

};

class Distributor : protected Consumer
{
private:
    /**
     * who this distributor was recruited by
     * nullptr if they directly bought in to the company
     */
    std::shared_ptr<Distributor> recruitedBy;
protected:
    /**
     * chance of making a sale to the other node
     * TODO: probably make this a static method that takes both the Consumer and Distributor
     * objects as parameters since the chance of a sale depends on factors from both
     */
    virtual double getSalesChance(const CapitalHolder& x) = 0;
    virtual bool canPurchase(const CapitalHolder& from, Money cost);

    bool isSubDistributor() { return recruitedBy.get() != nullptr; }
};


} //pyramid_scheme_simulator
