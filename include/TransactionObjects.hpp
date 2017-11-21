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



//can't use inheritance for the handler pattern because we're
//defining static methods

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

    using ElemType = Conversion;
    using RecordType = ListTransactionRecord<ElemType>;

    virtual RecordType operator()(SimulationTick,
            Money, 
            CapitalHolder&, 
            CapitalHolder&);


    using ComparatorType = const std::function<bool(const std::unique_ptr<ElemType>&, 
            const std::unique_ptr<ElemType>&)>;

    static ComparatorType comparator;

    static RecordType reduce(RecordType&&, RecordType&&);
};




class SaleHandler
{

public:
    class MoneyChangeRecord : public UniqueRecord
    {
    public:
        const Money fundsBefore;
        const Money fundsAfter;
        MoneyChangeRecord(SimulationTick, 
                Money price, 
                CapitalHolder*);
    };

    class Sale : public UniqueRecord
    {
    private:
        static Unique getUnique(SimulationTick, 
                Money price, 
                Distributor*, 
                Consumer*);
    public:
        const Money price;
        const MoneyChangeRecord sellerRecord;
        const MoneyChangeRecord buyerRecord;


        Sale(SimulationTick, 
                Money price, 
                const std::shared_ptr<Distributor>, 
                const std::shared_ptr<Consumer>);

        bool operator==(const Sale&);

    protected:
        Sale(const Sale&) = default;
    };

    using ElemType = Sale;
    using RecordType = ListTransactionRecord<ElemType>;

    virtual RecordType operator()(SimulationTick,
            Money, 
            CapitalHolder&, 
            CapitalHolder&);


    using ComparatorType = const std::function<bool(const std::unique_ptr<ElemType>&, 
            const std::unique_ptr<ElemType>&)>;

    static ComparatorType comparator;
    static RecordType reduce(RecordType&&, RecordType&&);
};

}
