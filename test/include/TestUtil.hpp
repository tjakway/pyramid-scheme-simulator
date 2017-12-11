#pragma once

#include "Util/Either.hpp"

#include <gtest/gtest.h>
#include <string>
#include <utility>

namespace {
    //ASSERT/EXPECT_PRED need a function whose return type is bool
    template <typename T>
    bool isInRange(T value, T le, T ge)
    {
        return le <= value && value <= ge;
    }
}

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
::testing::AssertionResult assertBetweenInclusive(T value, std::pair<T, T> range)
{
    return ASSERT_PRED3(isInRange, value, range.first, range.second);
}

}
