#pragma once

#include "Types.hpp"

namespace pyramid_scheme_simulator {

class Util
{
    /**
     * this class shouldn't be instantiated
     */
    Util() =delete;
public:
    /**
     * sample 1 bit using the passed RNG and the
     * chance that bit is 1
     */
    static bool sampleFrom(rd_ptr, double);
};

}
