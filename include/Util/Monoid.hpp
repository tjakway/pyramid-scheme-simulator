#pragma once

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

    virtual Monoid<T> mappend(Monoid<T>& other) = 0;

    /**
     * can be implemented as a performance optimization in cases where
     * the original could be moved instead of copied
     */
    virtual Monoid<T> mappend_move(Monoid<T>& other) {
        return mappend(other);
    }
};
