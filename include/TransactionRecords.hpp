#pragma once

#include "Util/Unique.hpp"
#include "Types.hpp"
#include "Tick.hpp"
#include "CapitalHolder.hpp"

#include <functional>
#include <memory>
#include <list>

namespace pyramid_scheme_simulator {


/************************************/

template <typename U>
class ListTransactionRecord 
{
protected:
    using ElementType = std::unique_ptr<U>;
    using ContainerType = std::list<ElementType>;
    ContainerType records;

public:
    ListTransactionRecord(ContainerType&& l) 
        : records(std::move(l.records))
    { }

    template <typename X>
    static ListTransactionRecord<X> mergeListTransactionRecords(
            ListTransactionRecord<X>&& lhs,
            ListTransactionRecord<X>&& rhs,
            std::function<bool(const std::unique_ptr<X>&, const std::unique_ptr<X>&)> 
                comparator)
    {
        std::list<std::unique_ptr<X>> combinedList = std::move(lhs.records);
        combinedList.merge(std::move(rhs.records), comparator);

        return ListTransactionRecord(combinedList);
    }
};



/**
 * returns true if fst < snd
 *
 * pass unique_ptrs by reference to prevent moves
 */
bool compareUniqueables(Uniqueable*,
        Uniqueable*);

}
