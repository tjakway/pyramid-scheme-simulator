#pragma once

#include "Util/Either.hpp"

#include <gtest/gtest.h>
#include <string>
#include <utility>

/*************************************************************************************************/

namespace __impl {
    /**
     * ******************WARNING******************: 
     * if you call this function and don't surround
     * it with an ASSERT_* macro, 
     *   ****IT WILL SILENTLY IGNORE FAILURES****
     *  
     *  for this reason it is ***ONLY*** intended
     *  to be used in this file in the function
     *  definition below
     */
    template <typename T>
    ::testing::AssertionResult assertInRangeInclusive(T value, std::pair<T, T> range)
    {
        const bool isInRange = range.first <= value && value <= range.second;

        if(isInRange)
        {
            return ::testing::AssertionSuccess();
        }
        else
        {
            return ::testing::AssertionFailure() << value << " is not within the range [" << 
                range.first << ", " << range.second << "]";
        }
    }
}
#define ASSERT_IN_RANGE_INCLUSIVE(value, range) ASSERT_TRUE(__impl::assertInRangeInclusive(value, range))

/*************************************************************************************************/




namespace pyramid_scheme_simulator {

template <typename L, typename R>
L assertLeft(Either<L, R> either, std::string msg = std::string())
{
    ASSERT_TRUE(either.isLeft()) << msg;
    return either.getLeft();
}

template <typename L, typename R>
R assertRight(Either<L, R> either, std::string msg = std::string())
{
    ASSERT_TRUE(either.isRight()) << msg;
    return either.getRight();
}



}
