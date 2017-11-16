#pragma once

#include "Types.hpp"

namespace pyramid_scheme_simulator {

class ChanceContributor
{
public:
    virtual double getChance(rd_ptr) = 0;

    static bool sampleFrom(rd_ptr, ChanceContributor&, ChanceContributor&);
};

}
