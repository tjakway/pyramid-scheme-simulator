#pragma once

#include "Util/Either.hpp"

#include <gtest/gtest.h>
#include <string>

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
