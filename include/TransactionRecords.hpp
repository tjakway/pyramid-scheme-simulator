#pragma once

#include "Util/Monoid.hpp"
#include "Util/Unique.hpp"
#include "Types.hpp"
#include "Tick.hpp"
#include "CapitalHolder.hpp"

#include <functional>
#include <memory>
#include <list>

namespace pyramid_scheme_simulator {

template <typename T> 
using TransactionRecord = Monoid<T>;

template <typename T, typename ...Us>
class Transaction
{
public:
    virtual TransactionRecord<T> operator()(Us... args) = 0;
};

template <typename T>
class VertexTransaction
    : public Transaction<T, SimulationTick, Money, CapitalHolder&>
{
public:
    virtual TransactionRecord<T> operator()(
            SimulationTick, Money, CapitalHolder&) = 0;
};

template <typename T>
class EdgeTransaction
    : public Transaction<T, SimulationTick, Money, CapitalHolder&, CapitalHolder&>
{
public:
    virtual TransactionRecord<T> operator()(SimulationTick,
            Money, 
            CapitalHolder&, 
            CapitalHolder&) = 0;
};

/************************************/

template <typename T>
class STLTransactionRecord
    : public TransactionRecord<T>
{
protected:
    virtual typename T::iterator begin() = 0;
    virtual typename T::iterator end() = 0;
public:

    virtual STLTransactionRecord<T> mappend(STLTransactionRecord<T>& other) override
    {
        T t = Monoid<T>::mempty();

        //copy self and other into the new container
        t.insert(begin(), end());
        t.insert(other.begin(), other.end());

        return t;
    }

    virtual STLTransactionRecord<T> mappend_move(STLTransactionRecord<T>& other) override
    {
        T t = Monoid<T>::mempty();

        //move self and other into the new container
        t.insert(std::make_move_iterator(begin()), 
                std::make_move_iterator(end()));
        t.insert(std::make_move_iterator(other.begin()), 
                std::make_move_iterator(other.end()));

        return t;
    }
};

template <typename U>
class ListTransactionRecord 
    : public STLTransactionRecord<std::list<std::unique_ptr<U>>>
{
protected:
    virtual bool cmp(std::unique_ptr<U>, std::unique_ptr<U>) = 0;
private:
    std::function<bool(std::unique_ptr<U>, std::unique_ptr<U>)>
        comparator = cmp;

    using SelfType = std::list<std::unique_ptr<U>>;
    SelfType records = Monoid<SelfType>::mempty();

    virtual typename SelfType::iterator begin() override {
        return records.begin();
    }
    virtual typename SelfType::iterator end() override {
        return records.end();
    }

public:
    ListTransactionRecord(std::initializer_list<SelfType> initValues)
        : records(initValues)
    {
        records.sort(comparator);
    }

    virtual ListTransactionRecord<SelfType> mappend_move(
                ListTransactionRecord<SelfType>& other) override
    {
        records.merge(other, comparator);
        return *this;
    }
};



/**
 * returns true if fst < snd
 *
 * pass unique_ptrs by reference to prevent moves
 */
bool compareUniqueables(std::unique_ptr<Uniqueable>&,
        std::unique_ptr<Uniqueable>&);

}
