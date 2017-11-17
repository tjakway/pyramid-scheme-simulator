#pragma once

#include "Types.hpp"
#include "Tick.hpp"
#include "CapitalHolder.hpp"
#include "Unique.hpp"

#include <memory>
#include <set>
#include <string>


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
    };

    /**
     * very frustrating that C++ can't do this automatically...
     */
    const char* reasonToString(Reason r) const
    {
        switch(r)
        {
            case UNKNOWN:
                return "UNKNOWN";
            case NEITHER_DISTRIBUTOR:
                return "NEITHER_DISTRIBUTOR";
            case CONSUMER_INSUFFICIENT_FUNDS:
                return "CONSUMER_INSUFFICIENT_FUNDS";
            case CHANCE_FAILED:
                return "CHANCE_FAILED";
            case SUCCESS:
                return "SUCCESS";
        }
        //keep it outside the switch so the compiler can warn us about
        //omitted cases when there isn't a default
        return "ERROR IN reasonToString--NO ENUM DEFINITION";
    }

    const Reason reason;

    /** whether or not a sale occurred */
    const bool success;

    explicit operator bool() const {
        return success;
    }

    bool operator!() const {
        return !success;
    }

    std::string str() const { return std::string(reasonToString(reason)); }

    SalesResult(Reason);
};


std::ostream& operator<<(std::ostream& os, const SalesResult& res)
{
    os << res.str();
    return os;
}


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

    class SaleIsPossibleResult;

    /**
     * return SalesResult objects to give more information about why the sale
     * didn't go through
     */
    SalesResult sampleSalesChance(rd_ptr, CapitalHolder& seller, CapitalHolder& buyer);
    SaleIsPossibleResult saleIsPossible(CapitalHolder& seller, CapitalHolder& buyer);

    void auditSales();
public:

    SalesResult processPotentialSale(SimulationTick,
            Money price,
            rd_ptr,
            CapitalHolder& seller,
            CapitalHolder& buyer);
     
};

}
