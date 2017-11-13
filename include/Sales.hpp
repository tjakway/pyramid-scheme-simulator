#pragma once

#include "Types.hpp"

#include <memory>
#include <set>


namespace pyramid_scheme_simulator {

class SalesResult
{
public:
    /**
     * TODO
     */
    enum Reason {
        UNKNOWN,
        CONSUMER_INSUFFICIENT_FUNDS,
        CHANCE_FAILED,
        SUCCESS
    } reason;

    const bool success;

    explicit operator bool() const {
        return success;
    }

    bool operator!() const {
        return !success;
    }
};

class Expenditure
{
public:
    const Money fundsBefore;
    const Money fundsAfter;
    Expenditure(Money, Money);
};

class Sale
{
public:
    const SimulationTick when;

    const std::shared_ptr<Distributor> seller;
    const std::shared_ptr<Consumer> buyer;
};

/**
 * global sales records
 */
class Transactions
{
    std::set<Sale> sales;

public:
    SalesResult sell(const CapitalHolder const& seller, const CapitalHolder const& buyer);
};

}
