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
    virtual bool canPurchase(const CapitalHolder const& from, unsigned int cost) {
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
public:
    /**
     * chance of making a sale to the other node
     */
    virtual double getSalesChance(const CapitalHolder const& x) = 0;
    virtual bool canPurchase(const CapitalHolder const& from, unsigned int cost);
};

} //pyramid_scheme_simulator
