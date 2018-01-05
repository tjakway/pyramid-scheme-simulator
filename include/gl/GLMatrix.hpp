#pragma once

#include "NamespaceDefines.hpp"

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp> 

BEGIN_PYRAMID_GL_NAMESPACE

class GLMatrix
{
    GLMatrix() = delete;
public:
    static const glm::vec3 origin;
    static const glm::mat4 identityMatrix;

    static glm::mat4 calculateWithModelMatrix(const glm::mat4&);

    static void updateMVPUniform(const glm::mat4&);
};

END_PYRAMID_GL_NAMESPACE
