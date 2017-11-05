#pragma once

#include "Config.hpp"

namespace pyramid_scheme_simulator {

class Unique;

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

    //TODO: private internal state
    
    //TODO: override equality operators
    

    //TODO: delete public constructors
};

}
