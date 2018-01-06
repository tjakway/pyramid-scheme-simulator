#include "gl/MainRenderer.hpp"

#include "gl/GLMatrix.hpp"

#include "Util/GeneralExceptions.hpp"
#include "Util/Strcat.hpp"

BEGIN_PYRAMID_GL_NAMESPACE

void MainRenderer::MatrixState::sizeCheck() const
{
    //(!) doesn't throw when empty
    //see http://www.cplusplus.com/reference/deque/deque/pop_front/
    if(matrixStack.size() < 1)
    {
        throw ImplementationException(STRCAT(__func__, " called with empty matrix stack"));
    }
}

MainRenderer::MatrixState::MatrixState()
{
    //the starting matrix is the identity matrix
    matrixStack.emplace_front(GLMatrix::identityMatrix);
}

glm::mat4 MainRenderer::MatrixState::getCurrentMatrix() const
{
    sizeCheck();
    return matrixStack.front();
}

void MainRenderer::MatrixState::setCurrentMatrix(const glm::mat4& x)
{
    //the current matrix is the one at the top of the stack
    if(matrixStack.size() > 0)
    {
        popMatrix();
        pushMatrix(x);
    }
    else
    {
        pushMatrix(x);
    }
}

void MainRenderer::MatrixState::pushMatrix(const glm::mat4& x)
{
    matrixStack.emplace_front(x);
}

glm::mat4 MainRenderer::MatrixState::popMatrix()
{
    sizeCheck();

    glm::mat4 top(matrixStack.front());
    matrixStack.pop_front();
    return top;
}

END_PYRAMID_GL_NAMESPACE
