#pragma once

//This file contains the objects that do the heavy lifting of transactions

#include "Types.hpp"
#include "Tick.hpp"
#include "CapitalHolder.hpp"
#include "Util/Unique.hpp"
#include "Util/Either.hpp"
#include "TransactionRecords.hpp"
#include "SalesResult.hpp"
#include "PopulationGraph.hpp"
#include "Actors.hpp"

#include <memory>
#include <set>
#include <string>
#include <unordered_set>
#include <initializer_list>
#include <functional>

namespace pyramid_scheme_simulator {



//can't use inheritance for the handlers because we're
//defining static methods

class ConversionHandler
{
    static bool testConversion(rd_ptr, 
            const Consumer&, 
            const Distributor&, 
            const Money buyIn);

    rd_ptr rd;
    const Money buyIn;

    /**
     * wraps testConversion and makes sure the 2 capitalholders
     * are a Consumer and Distributor
     */
    bool predF(const CapitalHolder&, const CapitalHolder&);
public:
    ConversionHandler(rd_ptr _rd, const Money _buyIn)
        : rd(_rd), buyIn(_buyIn) {}

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

    using ElementType = Conversion;
    using RecordType = ListTransactionRecord<ElementType>;

    virtual RecordType operator()(SimulationTick,
            CapitalHolder&, 
            CapitalHolder&);


    using ComparatorType = const std::function<bool(const std::unique_ptr<ElementType>&, 
            const std::unique_ptr<ElementType>&)>;

    static const ComparatorType comparator;

    static const std::function<RecordType(RecordType&&, RecordType&&)> reduce;

    const PopulationGraph::EdgePredicate predicate = 
        [this](const CapitalHolder& lhs, const CapitalHolder& rhs) { 
            return predF(lhs, rhs);
        };
};




class RestockHandler
{
public:
    using ElementType = Unique;
    using RecordType = ListTransactionRecord<ElementType>;

    //operates on vertices
    virtual RecordType operator()(const SimulationTick,
            const Money, 
            const CapitalHolder&);

    
    using SetComparatorType = const std::function<bool(const ElementType&,
            const ElementType&)>;
    static const SetComparatorType setComparator;
    
    using ListComparatorType = const std::function<bool(const std::unique_ptr<ElementType>&,
            const std::unique_ptr<ElementType>&)>;
    static const ListComparatorType listComparator;

    static const std::function<RecordType(RecordType&&, RecordType&&)> reduce;

    /**
     * the final output of this class
     */
    using RestockSet = std::set<ElementType>;
    static const RestockSet toSet(RecordType&&);
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
        Sale(const Sale&) = default;

        bool operator==(const Sale&);
    };

    using ElementType = Either<SalesResult, Sale>;
    using RecordType = ListTransactionRecord<ElementType>;

    SaleHandler(RestockHandler::RestockSet&&);

    virtual RecordType operator()(SimulationTick,
            Money, 
            rd_ptr,
            CapitalHolder&, 
            CapitalHolder&);


    using ComparatorType = const std::function<bool(const std::unique_ptr<ElementType>&, 
            const std::unique_ptr<ElementType>&)>;

    static const ComparatorType comparator;
    static const std::function<RecordType(RecordType&&, RecordType&&)> reduce;

protected:
    virtual RecordType doSale(SimulationTick,
            Money, 
            rd_ptr,
            CapitalHolder&, 
            CapitalHolder&);


private:
    class SaleIsPossibleResult;

    /**
     * return SalesResult objects to give more information about why the sale
     * didn't go through
     */
    SalesResult sampleSalesChance(rd_ptr, CapitalHolder& seller, CapitalHolder& buyer);
    SaleIsPossibleResult saleIsPossible(CapitalHolder& seller, CapitalHolder& buyer);
    SaleIsPossibleResult processPotentialSale(
            Money,
            rd_ptr,
            CapitalHolder&,
            CapitalHolder&);

    SalesResult needsRestock(Distributor&);

    RestockHandler::RestockSet restockSet;
};

class RestockSaleHandler
{
public:
    //thrown if a Unique in RestockSet doesn't
    //correspond to a vertex in PopulationGraph
    NEW_EXCEPTION_TYPE(RestockPopNotFoundException);

    RestockSaleHandler(const Money, 
            std::shared_ptr<Company>,
            RestockHandler::RestockSet);

    //synonym for processRestocks
    virtual SaleHandler::RecordType operator()(
                const SimulationTick,
                const PopulationGraph&,
                rd_ptr);

    SaleHandler::RecordType processRestocks(
            const SimulationTick, 
            const PopulationGraph&,
            rd_ptr);

private:
    std::shared_ptr<Company> company;
    const Money wholesalePrice;

    std::set<PopulationGraph::Pop> lookupRestockPops(
            const PopulationGraph&,
            const RestockHandler::RestockSet&);

    /**
     * called after we've looked up the Pops that need to be restocked
     */
    SaleHandler::RecordType processRestocksWithPops(
            const SimulationTick, 
            std::set<PopulationGraph::Pop> restockPops,
            rd_ptr);

    RestockHandler::RestockSet restockSet;
};

}
