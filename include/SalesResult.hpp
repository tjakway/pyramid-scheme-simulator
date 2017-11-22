#pragma once

#include "Types.hpp"
#include "Tick.hpp"
#include "CapitalHolder.hpp"
#include "Util/Unique.hpp"

#include <memory>
#include <set>
#include <string>
#include <unordered_set>
#include <initializer_list>

namespace pyramid_scheme_simulator {

/**
 * return SalesResult objects to give more information about why the sale
 * didn't go through
 */
class SalesResult
{
public:
    /**
     * TODO: add categories as needed
     */
    enum Reason {
        UNKNOWN,
        SELLER_NOT_DISTRIBUTOR,
        BUYER_NOT_CONSUMER,
        /** why would distributors sell to each other? */
        BOTH_DISTRIBUTORS,
        /** seller is out of inventory */
        NO_INVENTORY,
        /** distinct from NO_INVENTORY--
         * the distributor might have inventory but has
         * decided to restock this tick instead 
         *
         * for instance, a distributor might have 5 inventory
         * and 10 customers who want to buy
         * in this case he'll make 5 sales and 5 NO_INVENTORY
         * but next turn if he has 10 customers and decides to restock
         * all of his results will be NEEDS_RESTOCK*/
        NEEDS_RESTOCK,
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
            case SELLER_NOT_DISTRIBUTOR:
                return "SELLER_NOT_DISTRIBUTOR";
            case BUYER_NOT_CONSUMER:
                return "BUYER_NOT_CONSUMER";
            case BOTH_DISTRIBUTORS:
                return "BOTH_DISTRIBUTORS";
            case NO_INVENTORY:
                return "NO_INVENTORY";
            case NEEDS_RESTOCK:
                return "NEEDS_RESTOCK";
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


std::ostream& operator<<(std::ostream& os, const SalesResult& res);

}
