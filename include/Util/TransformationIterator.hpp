#pragma once

#include <functional>

template <typename It, typename T, typename U>
class TransformationIterator
{
    It it;
    const std::function<U(T)> transformIt;

public:
    TransformationIterator(const TransformationIterator& other)
        : it(other.it), transformIt(other.transformIt)
    {}

    TransformationIterator(It _it, 
            const std::function<U(T)> _transformIt)
        : it(_it), transformIt(_transformIt)
    {}

    //increment operators
    U& operator++()
    {
        // actual increment takes place here
        it++;
        return transformIt(it);
    }

    U operator++(int)
    {
        TransformationIterator tmp(*this); // copy
        operator++(); // pre-increment
        return tmp;   // return old value
    }

    //equality comparison operators
    inline bool operator==(const TransformationIterator& other)
    { 
        //do NOT compare member functions
        //see http://www.boost.org/doc/libs/1_55_0/doc/html/function/faq.html#idp97012536
        //for an explanation
        return other.it == it;
    }

    inline bool operator!=(const TransformationIterator& other)
    {
        return !operator==(other);
    }

    /**
     * NOTE: we can't return a reference here because we're passing the
     * underlying value of *it to a function and returning its
     * return value
     */
    U operator*()
    {
        return transformIt(*it);
    }
};
