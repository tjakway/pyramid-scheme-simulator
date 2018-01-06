#include "gl/VAOHandle.hpp"

#include "Util/GeneralExceptions.hpp"

#include "Util/Strcat.hpp"

#include <cassert>

BEGIN_PYRAMID_GL_NAMESPACE


//we're using triangle strips to draw a square, so need to specify
//attribute data for 4 vertices

std::array<float, VAOHandle::numColorDimensions> VAOHandle::defaultSolidColor = 
    {{ 0, 0, 0, 1 }};


std::array<float, VAOHandle::numPositionElems> VAOHandle::getPositionData() const
{
    return {{
        0, 0, 0,
        0, 1, 0,
        1, 0, 0,
        1, 1, 0
    }};
}

std::array<float, VAOHandle::numTexCoordElems> VAOHandle::getTexCoordData() const
{
    return {{
        0, 0, 
        0, 1, 
        1, 0, 
        1, 1, 
    }};
}

std::array<float, VAOHandle::numColorElems> VAOHandle::getColorData() const
{
    //unfortunately no easy way to flatten std::arrays at compile time in C++11
    //see https://stackoverflow.com/questions/25068481/c11-constexpr-flatten-list-of-stdarray-into-array
    return {{
        //vertex 1
        defaultSolidColor[0], 
            defaultSolidColor[1],
            defaultSolidColor[2],
            defaultSolidColor[3],

        //vertex 2
        defaultSolidColor[0], 
            defaultSolidColor[1],
            defaultSolidColor[2],
            defaultSolidColor[3],

        //vertex 3
        defaultSolidColor[0], 
            defaultSolidColor[1],
            defaultSolidColor[2],
            defaultSolidColor[3],

        //vertex 4
        defaultSolidColor[0], 
            defaultSolidColor[1],
            defaultSolidColor[2],
            defaultSolidColor[3],
    }};
}

namespace {
    template <typename It, typename T> 
    void takeFrom(
            It& from, 
            T& giveTo, 
            const int num, 
            //used to make sure the from iterator is valid
            const It& fromContainerEnd)
    {
        for(int i = 0; i < num; i++)
        {
            giveTo.emplace_back(*from);
            ++from;

            if(from == fromContainerEnd)
            {
                throw ImplementationException(STRCAT("Error in function ", 
                            __func__,
                            " in ", __FILE__, 
                            ": iterator is past the end"));
            }
        }
    }
}

std::vector<float> VAOHandle::interleaveVertexData()
{
    std::vector<float> vData;

    auto pData = getPositionData();
    auto tData = getTexCoordData();
    auto cData = getColorData();

    auto pIt = pData.cbegin(),
         pEnd = pData.cend();
    auto tIt = tData.cbegin(),
         tEnd = tData.cend();
    auto cIt = cData.cbegin(),
         cEnd = cData.cend();

    for(int i = 0; i < numVertices; i++)
    {
        takeFrom(pIt, vData, numPositionDimensions, pEnd);
        takeFrom(tIt, vData, numTexCoordDimensions, tEnd);
        takeFrom(cIt, vData, numColorDimensions, cEnd);
    }

    assert(vData.size() == numVBOElements);

    return vData;
}

END_PYRAMID_GL_NAMESPACE
