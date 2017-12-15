#pragma once

#include <functional>

namespace pyramid_scheme_simulator {

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
    TransformationIterator<It, T, U> operator++()
    {
        it++;
        return *this;
    }

    TransformationIterator<It, T, U> operator++(int)
    {
        TransformationIterator tmp(*this);
        operator++();
        return tmp;
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


/**
 * for type inference
 */
template <typename It, typename T, typename U>
TransformationIterator<It, T, U> mkTransformationIterator(It _it, 
            const std::function<U(T)> _transformIt)
{
    return TransformationIterator<It, T, U>(_it, _transformIt);
}

}
