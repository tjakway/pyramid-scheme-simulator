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
const int GLContext::openglRequiredMinorVersion = 2;

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

    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    std::cout << "glInit called" << std::endl;
}

void GLContext::glDraw()
{
    glClear(GL_COLOR_BUFFER_BIT);
    std::cout << "glDraw called" << std::endl;
}

void GLContext::glCleanup()
{
    std::cout << "glCleanup called" << std::endl;

}

END_PYRAMID_GL_NAMESPACE
