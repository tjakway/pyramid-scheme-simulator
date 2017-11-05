#pragma once

#include <set>

namespace pyramid_scheme_simulator {

class CapitalHolder;
class Company;
class Distributor;

class CapitalHolder 
{
    double money;

protected:
    void setMoney(double);
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
    double getSalesChance(const Node&);
};

} //pyramid_scheme_simulator
