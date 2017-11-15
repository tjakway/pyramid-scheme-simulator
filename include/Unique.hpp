#pragma once

#include "Guid.hpp"
#include "Types.hpp"

namespace pyramid_scheme_simulator {

class Uniqueable;

using Unique = xg::Guid;

Unique newUnique() { return xg::newGuid(); }

const static Unique emptyUnique = xg::Guid();

/**
 * a trait for classes with Unique IDs
 */
class Uniqueable 
{
public:
    const Unique id;
    Uniqueable() : id(xg::newGuid()) {}
    Uniqueable(Unique x) : id(x) {}
};

}
