#include "gl/MainRenderer.hpp"

#include "gl/GLMatrix.hpp"

BEGIN_PYRAMID_GL_NAMESPACE

void MainRenderer::glInit()
{
    shaderProgramHandle = ShaderProgramHandle::loadShaderProgramFromStrings(
                vertexShaderSource, fragmentShaderSource);

    //don't forget to set this as the current shader
    glUseProgram(shaderProgramHandle.get());

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    //set up the mvp matrix with no transformations yet
    GLMatrix::updateMVPUniform(GLMatrix::identityMatrix);
}

MainRenderer::MainRenderer(
        std::pair<std::unique_ptr<GraphLayout::Graph>, 
        GraphLayout::BoundingBox>)
{
    glInit();
}

void MainRenderer::draw()
{
    glClear(GL_COLOR_BUFFER_BIT);
}

END_PYRAMID_GL_NAMESPACE
