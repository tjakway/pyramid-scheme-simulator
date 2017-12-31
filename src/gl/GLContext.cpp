#include "gl/GLContext.hpp"

#include "NamespaceDefines.hpp"
#include "Util/Util.hpp"

#include <gtkmm.h>

BEGIN_PYRAMID_GL_NAMESPACE


const std::string GLContext::windowTitle = "Pyramid Scheme Simulator";
const std::string GLContext::applicationId = "pyramid_scheme_simulator"


GLContext::GLContext()
{
    windowData->app = 
        Gtk::Application::create(argc, argv,
            windowName);

    windowData->window.set_default_size(200, 200);
}

END_PYRAMID_GL_NAMESPACE
