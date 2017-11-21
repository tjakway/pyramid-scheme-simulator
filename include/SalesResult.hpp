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
