#include "Util/Util.hpp"

#include <limits>
#include <cmath>
#include <random>
#include <array>
#include <string>
#include <chrono>
#include <utility>

#include "Util/Strcat.hpp"

namespace {
std::uniform_real_distribution<double> getUniformRealDistZeroToOne()
{
    return std::uniform_real_distribution<double>(0.0, 1.0);
}
}

namespace pyramid_scheme_simulator {

/**
* if probability > 1 this will always return true
* (which is the expected behavior)
*/
bool Util::sampleFrom(rd_ptr rng, double probability)
{
    if(probability < 0.0)
    {
        return false;
    }
    else if(probability > 1.0)
    {
        throw SampleFromException(STRCAT("Cannot sample from a probability >1.0 but got",
                    probability));
    }
    else
    {
        //uniform distributions are allowed to be static because it shouldn't matter how many
        //times they've been sampled
        static auto dist = getUniformRealDistZeroToOne();

        return dist(*rng) <= probability;
    }
}

double Util::sampleUniformDistributionZeroToOne(rd_ptr rng)
{
    static auto dist = getUniformRealDistZeroToOne();
    return dist(*rng);
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


bool Util::withinMargins(double value, double expected, double margin)
{
    const auto margins = Util::getMargins(expected, margin);
    return margins.first <= value && value <= margins.second;
}


std::pair<double, double> Util::getMargins(double expected, double margin)
{
    const double max = (1.0 + (margin / 2.0)) * expected,
        min = (1.0 - (margin / 2.0)) * expected;

    return std::make_pair(min, max);
}

rd_ptr Util::rdSeededWithCurrentTime()
{
    return rdFromSeed(getCurrentTimeMillis());
}

rd_ptr Util::rdFromSeed(rd_seed_type seed)
{
    return std::make_shared<std::mt19937_64>(seed);
}

rd_seed_type Util::getCurrentTimeMillis()
{
    return static_cast<rd_seed_type>(std::chrono::duration_cast< 
            std::chrono::milliseconds >(
        std::chrono::system_clock::now().time_since_epoch()).count());
}

}
