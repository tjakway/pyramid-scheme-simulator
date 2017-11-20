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
    //need this to implement mappend and mappend_move in terms of STLTransactionRecord
    //despite this class being abstract
    virtual STLTransactionRecord* mkNew() = 0;

    virtual typename T::iterator begin() = 0;
    virtual typename T::iterator end() = 0;
public:

    virtual STLTransactionRecord<T>* mappend(TransactionRecord<T>& other) 
        override
    {
        std::unique_ptr<STLTransactionRecord<T>> t = 
            std::unique_ptr<STLTransactionRecord<T>>(mkNew());

        //copy self and other into the new container
        t->insert(begin(), end());
        t->insert(other.begin(), other.end());

        return t;
    }

    virtual STLTransactionRecord<T>* mappend_move(TransactionRecord<T>&& other) override
    {
        std::unique_ptr<STLTransactionRecord<T>> t = 
            std::unique_ptr<STLTransactionRecord<T>>(mkNew());

        //move self and other into the new container
        t.insert(std::make_move_iterator(begin()), 
                std::make_move_iterator(end()));
        t.insert(std::make_move_iterator(other.begin()), 
                std::make_move_iterator(other.end()));

        return t;
    }
};

// Monoid<ListTransactionRecord<U>>::mempty will function as 
// desired because begin() and end() are protected methods
// so mempty can be used to create a new ListTransactionRecord
// but not insert into it unless used within ListTransactionRecord itself
template <typename U>
class ListTransactionRecord 
    : public STLTransactionRecord<std::list<std::unique_ptr<U>>>
{
protected:
    virtual bool cmp(std::unique_ptr<U>, std::unique_ptr<U>) = 0;

private:
    std::function<bool(std::unique_ptr<U>, std::unique_ptr<U>)>
        comparator = cmp;

protected:
    using ElementType = std::unique_ptr<U>;
    using ContainerType = std::list<ElementType>;
    ContainerType records = Monoid<ContainerType>::mempty();

    //override begin and end to give STLTransactionRecord access to
    //this class' underlying container
    virtual typename ContainerType::iterator begin() override {
        return records.begin();
    }
    virtual typename ContainerType::iterator end() override {
        return records.end();
    }


    ListTransactionRecord(ContainerType&& l) : records(l)
    { }

public:
    ListTransactionRecord(std::initializer_list<ElementType> initValues)
        : records(initValues)
    {
        records.sort(comparator);
    }

    ListTransactionRecord() : records()
    { }

    virtual ListTransactionRecord<U>* mappend_move(
                ListTransactionRecord<U>&& other) override
    {
        records.merge(other, comparator);

        return make_unique(std::move(records));
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
