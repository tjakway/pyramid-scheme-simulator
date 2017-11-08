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
    double money;

protected:
    void setMoney(double);

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
    /**
     * describes the chance of making a sale to the given target
     */
    const std::function<double(const CapitalHolder const&)> saleChance;

public:
    /**
     * chance of making a sale to the other node
     */
    double getSalesChance(const CapitalHolder const& x) = saleChance(x);

    Distributor(std::function<double(const CapitalHolder const&)> f) : saleChance(f)
};

} //pyramid_scheme_simulator
