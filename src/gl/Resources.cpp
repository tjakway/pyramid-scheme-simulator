#include "gl/Resources.hpp"

BEGIN_PYRAMID_GL_NAMESPACE

static const std::string EXAMPLE_PNG = std::string("./example.png");

const std::string& Resources::getNodeTexturePath()
{
    return EXAMPLE_PNG;
}

END_PYRAMID_GL_NAMESPACE
