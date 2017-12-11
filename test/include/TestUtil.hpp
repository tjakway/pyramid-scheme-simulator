#pragma once

#include "Util/Either.hpp"

#include <gtest/gtest.h>
#include <string>
#include <utility>

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
