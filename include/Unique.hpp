#pragma once

#include "Guid.hpp"
#include "RDType.hpp"

namespace pyramid_scheme_simulator {

class Uniqueable;

using Unique = xg::Guid;

/**
 * a trait for classes with Unique IDs
 */
class Uniqueable 
{
public:
    const Unique id;
    Uniqueable() : id(xg::newGuid()) {}
};

}
