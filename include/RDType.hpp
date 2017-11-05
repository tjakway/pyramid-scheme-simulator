#pragma once

#include <random>
#include <memory>

namespace pyramid_scheme_simulator {
/**
 * type alias representing a pointer to a random generator
 */
using rd_ptr  = std::shared_ptr<std::mt19937_64>;
using rd_type = decltype(std::mt19937_64()());
}
