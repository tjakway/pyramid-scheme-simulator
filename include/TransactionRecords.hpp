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
private:
    const std::function<bool(std::unique_ptr<U>&, std::unique_ptr<U>&)>
        comparator;

protected:
    using ElementType = std::unique_ptr<U>;
    using ContainerType = std::list<ElementType>;
    ContainerType records;

    virtual typename ContainerType::iterator begin() {
        return records.begin();
    }
    virtual typename ContainerType::iterator end() {
        return records.end();
    }

    ListTransactionRecord(ContainerType&& l) 
        : records(l)
    { }

    virtual ListTransactionRecord* mkNew() = 0;
public:
    ListTransactionRecord(std::initializer_list<ElementType> initValues,
            std::function<bool(std::unique_ptr<U>&, std::unique_ptr<U>&)> comparator)
        : records(initValues), comparator(comparator)
    {
        records.sort(comparator);
    }

    ListTransactionRecord(std::function<bool(std::unique_ptr<U>&, std::unique_ptr<U>&)> 
            comparator) 
        : ListTransactionRecord({}, comparator)
    { }

    virtual ListTransactionRecord<U>* 
        mergeListTransactionRecords(ListTransactionRecord<U>&& other)
    {
        ListTransactionRecord<U>* t = mkNew();
        records.merge(other.records, comparator);
        t->records = records;
        return t;
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
