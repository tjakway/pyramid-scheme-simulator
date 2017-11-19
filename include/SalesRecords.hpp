#pragma once

#include "Types.hpp"
#include "Tick.hpp"
#include "CapitalHolder.hpp"
#include "Unique.hpp"

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


std::ostream& operator<<(std::ostream& os, const SalesResult& res);


class Recordable
{
protected:
    Recordable()=delete;
};

class UniqueRecord : public Uniqueable, Recordable
{
public:
    const SimulationTick when;
    UniqueRecord(SimulationTick when, Unique u)
        : Uniqueable(u), when(when) {}
};

class MoneyChangeRecord : public UniqueRecord
{
public:
    const Money fundsBefore;
    const Money fundsAfter;
    MoneyChangeRecord(SimulationTick, Money price, const std::shared_ptr<CapitalHolder>);
};


/**
 * don't make this an instance of UniqueRecord because it's a composite of 
 * a buyer record and a seller record and it's not obvious which id to assign it to
 */
class Sale
{
public:
    const Money price;
    const MoneyChangeRecord sellerRecord;
    const MoneyChangeRecord buyerRecord;

    Sale(SimulationTick, 
            Money price, 
            const std::shared_ptr<Distributor>, 
            const std::shared_ptr<Consumer>);
};

class Conversion : public UniqueRecord
{ 
public:
    const std::shared_ptr<Distributor> convertedBy;

    Conversion(SimulationTick when, 
            const Unique who, 
            std::shared_ptr<Distributor> convertedBy)
        : UniqueRecord(when, who), convertedBy(convertedBy)
    {}
};

}
