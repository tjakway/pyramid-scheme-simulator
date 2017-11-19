#pragma once

#include <memory>

template <typename T>
class Monoid
{
    /** default implementation uses no-args CTOR */
    template <typename U> static U mempty()
    {
        U x;
        return x;
    }

    virtual T mappend(T& other) = 0;

    /**
     * can be implemented as a performance optimization in cases where
     * the original could be moved instead of copied
     */
    virtual T mappend_move(T& other) {
        return mappend(other);
    }
};


template <typename T>
std::unique_ptr<T> Monoid::mempty<std::unique_ptr<T>>() {
    return std::unique_ptr<T>(nullptr);
}

