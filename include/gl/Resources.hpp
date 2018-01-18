#pragma once

#include <string>

#include "NamespaceDefines.hpp"

BEGIN_PYRAMID_GL_NAMESPACE


class Resources
{
    Resources() = delete;
public:
    static const std::string& getNodeTexturePath();
};

END_PYRAMID_GL_NAMESPACE
