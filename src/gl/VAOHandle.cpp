#include "gl/VAOHandle.hpp"

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

END_PYRAMID_GL_NAMESPACE
