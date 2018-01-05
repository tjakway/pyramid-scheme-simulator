#pragma once

#include "NamespaceDefines.hpp"

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp> 

BEGIN_PYRAMID_GL_NAMESPACE

class GLMatrix
{
public:
    glm::mat4 calculateWithModelMatrix(const glm::mat4&);
};

END_PYRAMID_GL_NAMESPACE