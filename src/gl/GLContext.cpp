#include "gl/GLContext.hpp"

#include "NamespaceDefines.hpp"
#include "Util/Util.hpp"

#include <gtkmm.h>

BEGIN_PYRAMID_GL_NAMESPACE

struct GLContext::WindowData
{
    std::unique_ptr<Gtk::Application> app;
    std::unique_ptr<Gtk::Window> window;
};

std::string GLContext::windowName = "Pyramid Scheme Simulator";


GLContext::GLContext(int argc, char** argv)
    : windowData(make_unique<WindowData>())
{
    windowData->app = std::unique_ptr<Gtk::Application>(
        Gtk::Application::create(argc, argv,
            windowName));

    windowData->window.set_default_size(200, 200);
}

void GLContext::run()
{
    return windowData->app->run(windowData->window);
}

END_PYRAMID_GL_NAMESPACE
