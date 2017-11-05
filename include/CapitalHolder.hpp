#pragma once

#include <set>

#include "Unique.hpp"

namespace pyramid_scheme_simulator {

class CapitalHolder;
class Company;
class Distributor;

class CapitalHolder : public Uniqueable
{
    double money;

protected:
    void setMoney(double);
};

/**
 * someone who hasn't been converted
 */
class Person
{

};

class Company
{
    /**
     * the company has a set of distributors working for them
     */
    std::set<Distributor> distributors;
};

class Distributor : protected CapitalHolder
{
public:
    /**
     * chance of making a sale to the other node
     */
    double getSalesChance(const CapitalHolder&);
};

} //pyramid_scheme_simulator
