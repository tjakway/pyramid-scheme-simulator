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
    unsigned int money;
    unsigned int productCost;

    double getMoneyToProductCostRatio();

    void setMoney(unsigned int);
    virtual bool canPurchase(const Distributor& from) {
        return money >= productCost;
    }

    virtual bool willPurchase(const Distributor& from) = 0;

    CapitalHolder(unsigned int cost) : Uniqueable(), productCost(cost) {}

    bool operator==(const CapitalHolder&);
    bool operator!=(const CapitalHolder&);
};

//TODO
class Consumer : protected CapitalHolder
{
protected:
    Consumer(unsigned int cost): CapitalHolder(cost) {}
public:
    void onBuy(const Distributor& from, SimulationTick when);

};

class Distributor : protected Consumer
{
protected:
    /**
     * chance of making a sale to the other node
     * TODO: probably make this a static method that takes both the Consumer and Distributor
     * objects as parameters since the chance of a sale depends on factors from both
     */
    virtual double getSalesChance(const CapitalHolder& x) = 0;
    virtual bool canPurchase(const CapitalHolder& from, unsigned int cost);
};


} //pyramid_scheme_simulator
