#include "gl/GLContext.hpp"

#include "NamespaceDefines.hpp"
#include "Util/Util.hpp"
#include "Util/Strcat.hpp"
#include "gl/GLUtil.hpp"
#include "gl/ShaderProgramHandle.hpp"
#include "gl/GLMatrix.hpp"

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
        const Config::BackendOptions::GLBackendOptions::WindowOptions& windowOptions,
        std::pair<const GraphLayout::Graph&, GraphLayout::BoundingBox> layoutParams)
    : glWindow(make_unique<GLWindow>(GLContext::windowTitle,
            std::make_pair(windowOptions.width, windowOptions.height),
            openglRequiredMajorVersion,
            openglRequiredMinorVersion))
{
    glWindow->makeCurrent();
    //only call glInit after we have an OpenGL context
    glInit(layoutParams);
}

void GLContext::glInit(
        std::pair<const GraphLayout::Graph&, GraphLayout::BoundingBox> layoutParams)
{
    //see https://www.opengl.org/discussion_boards/showthread.php/185079-glewExperimental
    glewExperimental = GL_TRUE;
    GLenum glewErr = glewInit();
    if(glewErr != GLEW_OK) {
        throw GLUtil::OpenGLException(STRCAT(
                    "Error occurred during GLEW initialization: ",
                    glewGetErrorString(glewErr)));
    }
    
    //TODO: pass CTOR args
    mainRenderer = new MainRenderer(layoutParams);
}

void GLContext::drawAndSwapFrames()
{
    mainRenderer->draw();
    glWindow->swapWindow();
}

std::shared_ptr<GLWindow::EventPoller> GLContext::getEventPoller()
{
    return glWindow->getEventPoller();
}

END_PYRAMID_GL_NAMESPACE
