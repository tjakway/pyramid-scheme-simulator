#pragma once

#include "RDType.hpp"

namespace pyramid_scheme_simulator {

class Unique;
class Uniqueable;

class UniqueGen 
{
private:
    const rd_ptr rd;

public:
    UniqueGen(rd_ptr p) : rd(p) {}
    Unique newUnique();
};

/**
 * a class that represents and generates unique IDs
 */
class Unique 
{
private:
    friend UniqueGen;

    rd_type id;
    Unique(rd_type x) : id(x) {}

public:
    bool operator==(const Unique& rhs){ this->id == rhs.id; }
    bool operator!=(const Unique& rhs){ return !(*this == rhs); }
};


/**
 * TODO: implement
 * a trait for classes with Unique IDs
 * TODO: ought to be pure virtual
 */
class Uniqueable 
{
    Unique id;
};

}
