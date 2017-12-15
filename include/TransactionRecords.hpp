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


template <typename X>
class ListTransactionRecord 
{
public:
    using ElementType = std::unique_ptr<X>;
    using ContainerType = std::list<ElementType>;
    ContainerType records;

    ListTransactionRecord(ContainerType&& l) 
        : records(std::move(l))
    { }

private:
    template <typename Container, typename Reducer>
    ListTransactionRecord<X>&& leftFoldHelper(Container toMerge, Reducer reducer, 
            ListTransactionRecord<X>&& acc)
    {
        if(toMerge.empty())
        {
            return std::move(acc);
        }
        else
        {
            ListTransactionRecord<X>&& newAcc = reducer(std::move(acc), toMerge.front);
            toMerge.pop_front();
            return leftFoldHelper(toMerge, reducer, std::move(newAcc));
        }
    }

public:
    //calls leftFoldHelper with this as the starting accumulator
    template <typename Container, typename Reducer>
    ListTransactionRecord<X>&& leftFold(Container toMerge, Reducer reducer)
    {
        return leftFoldHelper(toMerge, reducer, std::move(*this));
    }
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
 * returns a ListTransactonRecord containing 1 element
 * ! remember to pass a unique_ptr since ListTransactionRecorrd::ElementType is a unique_ptr<U>!
 */
template <typename X>
ListTransactionRecord<X> singleElementListTransactionRecord(
        typename ListTransactionRecord<X>::ElementType&& e)
{
    auto l = emptyListTransactionRecord<X>();
    l.records.emplace_back(std::move(e));
    return ListTransactionRecord<X>(std::move(l.records));
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
