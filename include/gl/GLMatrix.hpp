#pragma once

#include "NamespaceDefines.hpp"

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp> 

BEGIN_PYRAMID_GL_NAMESPACE

class GLMatrix
{
private:
    static glm::mat4 getViewMatrix(glm::vec3 viewDirection);
};

END_PYRAMID_GL_NAMESPACE
