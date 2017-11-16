#pragma once

#include "Types.hpp"
#include "Tick.hpp"
#include "CapitalHolder.hpp"
#include "Unique.hpp"

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
        NEITHER_DISTRIBUTOR,
        CONSUMER_INSUFFICIENT_FUNDS,
        CHANCE_FAILED,
        SUCCESS
    } reason;

    /** whether or not a sale occurred */
    const bool success;

    explicit operator bool() const {
        return success;
    }

    bool operator!() const {
        return !success;
    }
};

class MoneyChangeRecord
{
public:
    const Unique who;
    const Money fundsBefore;
    const Money fundsAfter;
    MoneyChangeRecord(Money price, const std::shared_ptr<CapitalHolder>);
};

class Sale
{
public:
    const SimulationTick when;

    const Money price;
    const MoneyChangeRecord sellerRecord;
    const MoneyChangeRecord buyerRecord;

    Sale(SimulationTick, 
            Money price, 
            const std::shared_ptr<Distributor>, 
            const std::shared_ptr<Consumer>);
};

/**
 * global sales records
 */
class Transactions
{
    std::set<Sale> sales;

    SalesResult sell(CapitalHolder& seller, CapitalHolder& buyer);
public:
    SalesResult processSalesChance(rd_ptr, CapitalHolder& seller, CapitalHolder& buyer);
     
};

}
