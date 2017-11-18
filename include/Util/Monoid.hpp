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

    T mappend_move(T& other) {
        return mappend(other);
    }
};
