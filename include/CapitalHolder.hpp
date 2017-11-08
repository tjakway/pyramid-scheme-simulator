#pragma once

#include <set>
#include <vector>
#include <functional>

#include "Unique.hpp"
#include "Tick.hpp"

namespace pyramid_scheme_simulator {

class CapitalHolder;
class Company;
class Distributor;

class CapitalHolder : public Uniqueable
{
    unsigned int money;

protected:
    void setMoney(unsigned int);
    bool canPurchase(unsigned int cost) {
        return money >= cost;
    }

public:
    CapitalHolder() : Uniqueable() {}
};


class Sale
{
    const SimulationTick when;
    const std::shared_ptr<Distributor> boughtFrom;
};

//TODO
class Consumer : protected CapitalHolder
{
    std::set<Sale> purchases;

public:
    void buy(const Distributor const& from, SimulationTick when);
};


class Distributor : protected Consumer
{
    using SalesFunction = std::function<double(const CapitalHolder const&)>

    /**
     * describes the chance of making a sale to the passed target
     */
    const SalesFunction saleChance;

public:
    /**
     * chance of making a sale to the other node
     */
    double getSalesChance(const CapitalHolder const& x) = saleChance(x);

    Distributor(SalesFunction f) : saleChance(f)
};

} //pyramid_scheme_simulator
