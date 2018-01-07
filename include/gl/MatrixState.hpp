#pragma once

#include "NamespaceDefines.hpp"

#include "glm/matrix.hpp"

#include <deque>

BEGIN_PYRAMID_GL_NAMESPACE

class MatrixState
{
    void sizeCheck() const;
    std::deque<glm::mat4> matrixStack;
public:
    MatrixState();

    glm::mat4 getCurrentMatrix() const;
    void setCurrentMatrix(const glm::mat4&);

    glm::mat4 popMatrix();
    void pushMatrix();
};

END_PYRAMID_GL_NAMESPACE
