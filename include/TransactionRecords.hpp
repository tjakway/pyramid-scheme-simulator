#pragma once

#include "Util/Unique.hpp"
#include "Types.hpp"
#include "Tick.hpp"
#include "CapitalHolder.hpp"

#include <functional>
#include <memory>
#include <list>

namespace pyramid_scheme_simulator {


template <typename U>
class ListTransactionRecord 
{
public:
    using ElementType = std::unique_ptr<U>;
    using ContainerType = std::list<ElementType>;
    ContainerType records;

    ListTransactionRecord(ContainerType&& l) 
        : records(std::move(l))
    { }

};

template <typename X>
ListTransactionRecord<X> mergeListTransactionRecords(
        ListTransactionRecord<X>&& lhs,
        ListTransactionRecord<X>&& rhs,
        std::function<bool(const std::unique_ptr<X>&, const std::unique_ptr<X>&)> 
            comparator)
{
    std::list<std::unique_ptr<X>> combinedList = std::move(lhs.records);
    combinedList.merge(std::move(rhs.records), comparator);

    return ListTransactionRecord<X>(std::move(combinedList));
}


/**
 * returns true if fst < snd
 *
 * pass unique_ptrs by reference to prevent moves
 */
bool compareUniqueables(Uniqueable*,
        Uniqueable*);


//data base classes
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


class MoneyChangeRecord : public UniqueRecord
{
public:
    const Money fundsBefore;
    const Money fundsAfter;
    MoneyChangeRecord(SimulationTick, Money price, const std::shared_ptr<CapitalHolder>);
};

class Sale : public UniqueRecord
{
private:
    static Unique getUnique(SimulationTick, 
            Money price, 
            const std::shared_ptr<Distributor>, 
            const std::shared_ptr<Consumer>);
public:
    const Money price;
    const MoneyChangeRecord sellerRecord;
    const MoneyChangeRecord buyerRecord;


    Sale(SimulationTick, 
            Money price, 
            const std::shared_ptr<Distributor>, 
            const std::shared_ptr<Consumer>);
};

}
