#include "Util/Util.hpp"

#include <limits>
#include <cmath>
#include <random>
#include <array>
#include <string>

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

        return dist(*rng) <= probability;
    }
}

const std::array<unsigned char, 16> Util::hashToArray(size_t hash)
{
    std::string hashStr(std::to_string(hash));

    //pad the string to size 16
    while(hashStr.length() < 16)
    {
        hashStr.push_back('0');
    }

    return {{
        (unsigned char)hashStr[0],
        (unsigned char)hashStr[1],
        (unsigned char)hashStr[2],
        (unsigned char)hashStr[3],
        (unsigned char)hashStr[4],
        (unsigned char)hashStr[5],
        (unsigned char)hashStr[6],
        (unsigned char)hashStr[7],
        (unsigned char)hashStr[8],
        (unsigned char)hashStr[9],
        (unsigned char)hashStr[10],
        (unsigned char)hashStr[11],
        (unsigned char)hashStr[12],
        (unsigned char)hashStr[13],
        (unsigned char)hashStr[14],
        (unsigned char)hashStr[15],
    }};
}

}
