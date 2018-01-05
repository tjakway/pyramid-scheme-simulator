#include "gl/GLMatrix.hpp"
#include "gl/ShaderDefines.hpp"

#include <GL/glew.h>

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp> 
#include <glm/gtc/matrix_transform.hpp>

BEGIN_PYRAMID_GL_NAMESPACE

const glm::vec3 GLMatrix::origin = glm::vec3(0, 0, 0);
const glm::mat4 GLMatrix::identityMatrix = glm::mat4(1.0f);

static const glm::mat4 startingProjection = glm::ortho(-1.0, 1.0, -1.0, 1.0);

static const glm::mat4 startingCamera = glm::lookAt(
        glm::vec3(0, 0, -1), //camera is at x=0, y=0, z=-1
        glm::vec3(0, 0, 0),  //looks at origin
        glm::vec3(0, 1, 0)   //positive y = up
        );

static const glm::mat4 calculateMVP(
        const glm::mat4& projection, 
        const glm::mat4& view,
        const glm::mat4& model)
{
    return projection * view * model;
}

glm::mat4 GLMatrix::calculateWithModelMatrix(const glm::mat4& model)
{
    return calculateMVP(startingProjection, startingCamera, model);
}

void GLMatrix::updateMVPUniform(const glm::mat4& matrix)
{
    glUniformMatrix4fv(VS_MVP_LOC, 1, GL_FALSE, &matrix[0][0]);
}

END_PYRAMID_GL_NAMESPACE
