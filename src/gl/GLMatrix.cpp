#include "gl/GLMatrix.hpp"

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp> 

BEGIN_PYRAMID_GL_NAMESPACE

static const glm::vec3 origin = glm::vec3(0, 0, 0);

glm::mat4 GLMatrix::getViewMatrix(glm::vec3 viewDirection)
{
    const glm::vec3 upDirection = glm::vec3(0, 1, 0);

    const glm::vec3 zDirection = glm::inverse(glm::normalize(viewDirection));

    const glm::vec3 xDirection = 
        (viewDirection * upDirection) / glm::abs(viewDirection * upDirection);

    const glm::vec3 yDirection = zDirection * xDirection;

    const glm::mat3 rMatrix = glm::mat3(xDirection, yDirection, zDirection);

    const glm::mat3 rMatrixTransposed = glm::transpose(rMatrix);


    const glm::vec4 col1 = glm::vec4(rMatrixTransposed[0], 0),
          col2 = glm::vec4(rMatrixTransposed[1], 0),
          col3 = glm::vec4(rMatrixTransposed[2], 0),
          col4 = glm::vec4(rMatrixTransposed * origin, 1);

    return glm::mat4(col1, col2, col3, col4);
}

END_PYRAMID_GL_NAMESPACE
