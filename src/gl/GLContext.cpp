#include "gl/GLContext.hpp"

#include "NamespaceDefines.hpp"
#include "Util/Util.hpp"

#include <utility>
#include <functional>

#include <gtkmm.h>

BEGIN_PYRAMID_GL_NAMESPACE


const std::string GLContext::windowTitle = "Pyramid Scheme Simulator";
const std::string GLContext::applicationId = "pyramid_scheme_simulator"


GLContext::GLContext(
        const Config::BackendOptions::GLBackendOptions::WindowOptions& windowOptions)
    : glWindow(make_unique<GLWindow>(GLContext::windowTitle,
            std::make_pair(windowOptions.width, windowOptions.height),
            std::bind(glInit, this),
            std::bind(glDraw, this),
            std::bind(glCleanup, this),
            openglRequiredMajorVersion = -1,
            openglRequiredMinorVersion = -1))
{}


void GLContext::run(Glib::RefPtr<Application> app)
{
    glWindow->run(app, GLContext::applicationId);
}

void GLContext::run()
{
    const auto app = Gtk::Application::create(
            GLContext::applicationId);
    run(app);
}

END_PYRAMID_GL_NAMESPACE
