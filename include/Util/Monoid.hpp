#pragma once

#include <memory>

template <typename T>
class Monoid
{
public:
    template <typename U> static Monoid<U>* mempty();

    virtual Monoid<T>* mappend(Monoid<T>& other) = 0;

    /**
     * can be implemented as a performance optimization in cases where
     * the original could be moved instead of copied
     */
    virtual Monoid<T>* mappend_move(Monoid<T>&& other) = 0;
};
