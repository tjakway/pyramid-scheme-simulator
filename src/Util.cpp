#include "Util.hpp"

#include <limits>
#include <cmath>
#include <random>

namespace pyramid_scheme_simulator {

/**
 * if probability > 1 this will always return true
 * (which is the expected behavior)
 */
bool Util::sampleFrom(rd_ptr rng, double probability)
{
    if(probability < 0)
    {
        return false;
    }
    else
    {
        //XXX: maybe overcomplicating this
        //sample on the interval [0, 1] instead of [0, 1)

        static auto dist = std::uniform_real_distribution<double>(0.0, 
                std::nextafter(1.0, std::numeric_limits<double>::max()));

        return dist(rng) <= probability;
    }
}

}
