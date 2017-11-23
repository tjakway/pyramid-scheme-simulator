#pragma once

#include "Util/Unique.hpp"
#include "Types.hpp"
#include "Tick.hpp"
#include "CapitalHolder.hpp"

#include <functional>
#include <memory>
#include <list>
#include <utility>
#include <set>

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
ListTransactionRecord<X> emptyListTransactionRecord() 
{
    typename ListTransactionRecord<X>::ContainerType emptyContainer;
    return ListTransactionRecord<X>(std::move(emptyContainer));
}

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

bool compareUniques(const Unique*, const Unique*);

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

    bool operator==(const UniqueRecord&);
};

}
