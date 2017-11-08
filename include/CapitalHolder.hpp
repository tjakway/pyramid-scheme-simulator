#pragma once

#include <set>
#include <vector>

#include "Unique.hpp"
#include "Simulation.hpp"

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

class Company : protected CapitalHolder
{
    /**
     * the company has a set of distributors working for them
     */
    std::set<Distributor> distributors;
};


class Sale
{
    const Simulation::tick when;
    const std::shared_ptr<Distributor> boughtFrom;
};

//TODO
class Consumer : protected CapitalHolder
{
    std::set<Sale> purchases;

};


class Distributor : protected Consumer
{
public:
    /**
     * chance of making a sale to the other node
     */
    double getSalesChance(const CapitalHolder&);
};

} //pyramid_scheme_simulator
