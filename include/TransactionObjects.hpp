#pragma once

//This file contains the objects that do the heavy lifting of transactions

#include "Types.hpp"
#include "Tick.hpp"
#include "CapitalHolder.hpp"
#include "Util/Unique.hpp"
#include "TransactionRecords.hpp"

#include <memory>
#include <set>
#include <string>
#include <unordered_set>
#include <initializer_list>

namespace pyramid_scheme_simulator {

//data classes
class Recordable
{
protected:
    Recordable() = default;
};

class UniqueRecord : public Uniqueable, Recordable
{
public:
    const SimulationTick when;
    UniqueRecord(SimulationTick when, Unique u)
        : Uniqueable(u), when(when) {}
};



//transaction classes
class ConversionTransaction
{
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

    class ConversionRecords : public ListTransactionRecord<Conversion>
    {
    protected:
        virtual bool cmp(std::unique_ptr<Conversion>, 
                std::unique_ptr<Conversion>);
    };

    class ConversionTransactionHandler : public EdgeTransaction<Conversion>
    {
        virtual ConversionRecords operator()(SimulationTick,
                Money, 
                CapitalHolder&, 
                CapitalHolder&) override;
    };

};


}
