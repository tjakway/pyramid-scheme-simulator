#include "gl/GLContext.hpp"

#include "NamespaceDefines.hpp"
#include "Util/Util.hpp"
#include "Util/Strcat.hpp"
#include "gl/GLUtil.hpp"
#include "gl/ShaderProgramHandle.hpp"

#include <utility>
#include <functional>
#include <atomic>

#include <GL/glew.h>

BEGIN_PYRAMID_GL_NAMESPACE

const std::string GLContext::windowTitle = "Pyramid Scheme Simulator";
const std::string GLContext::applicationId = "pyramid_scheme_simulator";

const int GLContext::openglRequiredMajorVersion = 4;
const int GLContext::openglRequiredMinorVersion = 5;

GLContext::GLContext(
        const Config::BackendOptions::GLBackendOptions::WindowOptions& windowOptions)
    : glWindow(make_unique<GLWindow>(GLContext::windowTitle,
            std::make_pair(windowOptions.width, windowOptions.height),
            std::bind(&GLContext::glInit, this),
            std::bind(&GLContext::glDraw, this),
            std::bind(&GLContext::glCleanup, this),
            openglRequiredMajorVersion,
            openglRequiredMinorVersion))
{}


void GLContext::run()
{
    glWindow->run();
}

static GLuint TEST_VAO;

#include <iostream>
void GLContext::glInit()
{
    //see https://www.opengl.org/discussion_boards/showthread.php/185079-glewExperimental
    glewExperimental = GL_TRUE;
    GLenum glewErr = glewInit();
    if(glewErr != GLEW_OK) {
        throw GLUtil::OpenGLException(STRCAT(
                    "Error occurred during GLEW initialization: ",
                    glewGetErrorString(glewErr)));
    }
    

    shaderProgramHandle = ShaderProgramHandle::loadShaderProgramFromStrings(
                vertexShaderSource, fragmentShaderSource);

    //don't forget to set this as the current shader
    glUseProgram(shaderProgramHandle.get());


    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    float points[] = {
        0.0f,  0.5f,  0.0f,
        0.5f, -0.5f,  0.0f,
        -0.5f, -0.5f,  0.0f
    };

    GLuint vbo = 0;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, 9 * sizeof(float), points, GL_STATIC_DRAW);

    GLuint vao = 0;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    TEST_VAO = vao;
}

void GLContext::glDraw()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glDrawArrays(GL_TRIANGLES, 0, 3);
}

void GLContext::glCleanup()
{ 
//    glDeleteVertexArrays(1, &TEST_VAO);
}

END_PYRAMID_GL_NAMESPACE
