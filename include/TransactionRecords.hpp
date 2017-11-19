#pragma once

#include "Util/Monoid.hpp"
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
{
public:
    virtual TransactionRecord<T> operator()(CapitalHolder&) = 0;
};

template <typename T>
class EdgeTransaction
{
public:
    virtual TransactionRecord<T> operator()(CapitalHolder&,
                                            CapitalHolder&) = 0;
};

/************************************/

template <typename T>
class STLTransactionRecord
    : public TransactionRecord<T>
{
protected:
    virtual T::iterator begin() = 0;
    virtual T::iterator end() = 0;
public:

    virtual T mappend(T& other) override
    {
        T t = Monoid<T>::mempty();

        //copy self and other into the new container
        t.insert(begin(), end());
        t.insert(other.begin(), other.end());

        return t;
    }

    virtual T mappend_move(T& other) override
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
    : public TransactionRecord<std::list<std::unique_ptr<U>>>
{
protected:
    virtual bool cmp(std::unique_ptr<U>, std::unique_ptr<U>) = 0;
private:
    std::function<bool(std::unique_ptr<U>, std::unique_ptr<U>)>
        comparator = cmp;
public:
    using SelfType = std::list<std::unique_ptr<U>>;

    SelfType records = Monoid::mempty();


    virtual SelfType mappend_move(SelfType& other)
    {
        //TODO: reduce redundant sorting
        records.sort(comparator);
        other.sort(comparator);

        records.merge(other, comparator);
    }
};


}
