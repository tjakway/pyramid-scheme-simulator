#include "gl/MatrixState.hpp"

#include "gl/GLMatrix.hpp"

#include "Util/GeneralExceptions.hpp"
#include "Util/Strcat.hpp"

BEGIN_PYRAMID_GL_NAMESPACE

void MatrixState::sizeCheck() const
{
    //(!) doesn't throw when empty
    //see http://www.cplusplus.com/reference/deque/deque/pop_front/
    if(matrixStack.size() < 1)
    {
        throw ImplementationException(STRCAT(__func__, " called with empty matrix stack"));
    }
}

MatrixState::MatrixState()
{
    //the starting matrix is the identity matrix
    matrixStack.emplace_front(GLMatrix::identityMatrix);
}

glm::mat4& MatrixState::getCurrentMatrix() const
{
    sizeCheck();
    return matrixStack.front();
}

void MatrixState::setCurrentMatrix(const glm::mat4& x)
{
    //the current matrix is the one at the top of the stack
    if(matrixStack.size() > 0)
    {
        popMatrix();
        matrixStack.emplace_front(x);
    }
    else
    {
        matrixStack.emplace_front(x);
    }
}

void MatrixState::pushMatrix()
{
    matrixStack.emplace_front(getCurrentMatrix());
}

glm::mat4 MatrixState::popMatrix()
{
    sizeCheck();

    glm::mat4 top(matrixStack.front());
    matrixStack.pop_front();
    return top;
}

END_PYRAMID_GL_NAMESPACE
