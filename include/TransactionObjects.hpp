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




//transaction classes
class ConversionHandler
{
public:

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

    using RecordType = ListTransactionRecord<Conversion>;

    virtual RecordType operator()(SimulationTick,
            Money, 
            CapitalHolder&, 
            CapitalHolder&);


    using ComparatorType = const std::function<bool(const std::unique_ptr<Conversion>&, 
            const std::unique_ptr<Conversion>&)>;

    static ComparatorType comparator;

    static RecordType reduce(RecordType&&, RecordType&&);
};


}
