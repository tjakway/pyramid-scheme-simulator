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
    /**
     * either contains the Conversion::RecordType or
     * provides information on why a conversion didn't happen
     *
     * move the class definition here if we decide to log this information
     */
    class ConversionPredicateResult;

    static ConversionPredicateResult testConversion(rd_ptr, 
            const SimulationTick when,
            const Consumer&, 
            const Distributor&, 
            const Money buyIn);

    rd_ptr rd;
    const Money buyIn;

    /**
     * wraps testConversion and makes sure the 2 capitalholders
     * are a Consumer and Distributor
     */
    ConversionPredicateResult predF(const SimulationTick when, const CapitalHolder&, const CapitalHolder&);
public:
    ConversionHandler(rd_ptr _rd, const Money _buyIn);

    class Conversion : public UniqueRecord
    { 
    public:
        const Unique convertedBy;

        Conversion(SimulationTick when, 
                const Unique who, 
                const Unique& _convertedBy)
            : UniqueRecord(when, who), convertedBy(_convertedBy)
        {}
    };

    using ElementType = Conversion;
    using RecordType = ListTransactionRecord<ElementType>;

    virtual RecordType operator()(SimulationTick,
            CapitalHolder&, 
            CapitalHolder&);

    
    /**
     * synonym for the other function operator but defined to work on graph pairs
     */
    virtual ConversionHandler::RecordType operator(SimulationTick, 
            std::pair<PopulationGraph::Pop, PopulationGraph::Pop>);


    using ComparatorType = const std::function<bool(const std::unique_ptr<ElementType>&, 
            const std::unique_ptr<ElementType>&)>;

    static const ComparatorType comparator;

    static const std::function<RecordType(RecordType&&, RecordType&&)> reduce;

    PopulationGraph::EdgePredicate getPredicate(const SimulationTick when);
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
        const std::shared_ptr<Distributor> seller;
        const std::shared_ptr<Consumer> buyer;

        Sale(SimulationTick, 
                Money price, 
                const std::shared_ptr<Distributor>, 
                const std::shared_ptr<Consumer>);
        Sale(const Sale&) = default;

        bool operator==(const Sale&);
    };

    using ElementType = Either<SalesResult, Sale>;
    using RecordType = ListTransactionRecord<ElementType>;

    SaleHandler(const RestockHandler::RestockSet&);

    virtual ~SaleHandler() {}

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
            const RestockHandler::RestockSet&);

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
