#pragma once

#include <random>
#include <iterator>
#include <array>

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

    /**
     * from https://stackoverflow.com/questions/6942273/get-random-element-from-container
     */
    template<typename Iter, typename RandomGenerator>
    static Iter selectRandomly(Iter start, Iter end, RandomGenerator& g) {
        std::uniform_int_distribution<> dis(0, std::distance(start, end) - 1);
        std::advance(start, dis(g));
        return start;
    }


    template <typename T>
    static bool deepCompareUniquePtrs(const std::unique_ptr<T>& lhs, const std::unique_ptr<T>& rhs)
    {
        //compare pointer values first
        if(lhs.get() == nullptr && rhs.get() == nullptr)
        {
            return true;
        }
        else if(lhs.get() == nullptr && rhs.get() != nullptr)
        {
            return false;
        }
        else if(lhs.get() != nullptr && rhs.get() == nullptr)
        {
            return false;
        }
        else
        {
            //compare referands
            return *lhs == *rhs;
        }
    }

    static const std::array<unsigned char, 16> hashToArray(size_t hash);
};


template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args)
{
    return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}

}
