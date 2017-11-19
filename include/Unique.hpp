#pragma once

#include "Guid.hpp"
#include "Types.hpp"

namespace pyramid_scheme_simulator {

class Uniqueable;

class Unique
{
    xg::Guid guid;
protected:
    Unique(xg::Guid guid): guid(guid) {}

public:
    static Unique newUnique() { 
        return Unique(xg::newGuid()); 
    }
    static const Unique emptyUnique = xg::Guid();
};


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
