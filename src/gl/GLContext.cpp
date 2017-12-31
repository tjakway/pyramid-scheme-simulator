#include "gl/GLContext.hpp"

#include "NamespaceDefines.hpp"
#include "Util/Util.hpp"

#include <utility>
#include <functional>

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
            openglRequiredMajorVersion = -1,
            openglRequiredMinorVersion = -1))
{}


void GLContext::run(Glib::RefPtr<Gtk::Application> app)
{
    glWindow->run(app, GLContext::applicationId.c_str());
}

void GLContext::run()
{
    const auto app = Gtk::Application::create(
            GLContext::applicationId);
    run(app);
}

END_PYRAMID_GL_NAMESPACE
