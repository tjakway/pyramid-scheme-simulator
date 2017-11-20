#pragma once

#include <memory>

template <typename T>
class Monoid
{
public:
    /** default implementation uses no-args CTOR */
    template <typename U> static U mempty()
    {
        U x;
        return x;
    }

    virtual std::unique_ptr<T> mappend(T& other) = 0;

    /**
     * can be implemented as a performance optimization in cases where
     * the original could be moved instead of copied
     */
    virtual std::unique_ptr<T> mappend_move(T&& other) {
        return mappend(other);
    }
};
