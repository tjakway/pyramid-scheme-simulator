#include "gl/VAOHandle.hpp"

BEGIN_PYRAMID_GL_NAMESPACE


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

}

END_PYRAMID_GL_NAMESPACE
