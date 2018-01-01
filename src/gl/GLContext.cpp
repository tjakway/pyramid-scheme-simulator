#include "gl/GLContext.hpp"

#include "NamespaceDefines.hpp"
#include "Util/Util.hpp"
#include "gl/GLUtil.hpp"

#include <utility>
#include <functional>

#include <GL/glew.h>
#include <gtkmm.h>

BEGIN_PYRAMID_GL_NAMESPACE


const std::string GLContext::windowTitle = "Pyramid Scheme Simulator";
const std::string GLContext::applicationId = "pyramid_scheme_simulator";

const int GLContext::openglRequiredMajorVersion = -1;
const int GLContext::openglRequiredMinorVersion = -1;

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


void GLContext::run(Glib::RefPtr<Gtk::Application> app)
{
    glWindow->run(app);
}

void GLContext::run()
{
    const auto app = Gtk::Application::create(
            GLContext::applicationId);
    run(app);
}

#include <iostream>

void GLContext::glInit()
{
    //see https://www.opengl.org/discussion_boards/showthread.php/185079-glewExperimental
    glewExperimental = GL_TRUE;
    if(glewInit() != GLEW_OK) {
        throw GLUtil::OpenGLException("Error occurred during GLEW initialization");
    }

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
