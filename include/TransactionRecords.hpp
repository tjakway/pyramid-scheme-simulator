#pragma once

#include "Util/Monoid.hpp"
#include "CapitalHolder.hpp"

#include <unordered_set>
#include <memory>

namespace pyramid_scheme_simulator {

template <typename T> 
using TransactionRecord = Monoid<T>;

template <typename T, typename ...Us>
class Transaction
{
    virtual TransactionRecord<T> operator()(Us... args) = 0;
};

template <typename T>
class VertexTransaction
{
    virtual TransactionRecord<T> operator()(CapitalHolder&) = 0;
};

template <typename T>
class EdgeTransaction
{
    virtual TransactionRecord<T> operator()(CapitalHolder&,
                                            CapitalHolder&) = 0;
};

/************************************/

template <typename T>
class UnorderedSetTransactionRecord 
    : public TransactionRecord<
        std::unordered_set<
         std::unique_ptr<T>>>
{
    using SelfType = std::unordered_set<std::unique_ptr<T>>;

    virtual SelfType mappend(SelfType& other) override
    {
        SelfType t = Monoid<SelfType>::mempty();

        //copy self and other into the new container
        auto self = getSelf();
        t.insert(self.begin(), self.end());
        t.insert(other.begin(), other.end());

        return t;
    }

    virtual SelfType mappend_move(SelfType& other) override
    {
        SelfType t = Monoid<SelfType>::mempty();

        //move self and other into the new container
        auto self = getSelf();
        t.insert(std::make_move_iterator(self.begin()), 
                std::make_move_iterator(self.end()));
        t.insert(std::make_move_iterator(other.begin()), 
                std::make_move_iterator(other.end()));

        return t;
    }
};


}
