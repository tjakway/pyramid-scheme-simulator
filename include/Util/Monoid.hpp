#pragma once

template <typename T>
class Monoid
{
    /** default implementation uses no-args CTOR */
    template <typename U> static U mempty()
    {
        U x;
        return x;
    }

    T mappend(T& other);

    /**
     * can be implemented as a performance optimization in cases where
     * the original could be moved instead of copied
     */
    T mappend_move(T& other) {
        return mappend(other);
    }
};
