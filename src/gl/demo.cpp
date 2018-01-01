#include <iostream>

#include "Config.hpp"
#include "gl/GLContext.hpp"

#include <gtkmm.h>

using namespace pyramid_scheme_simulator;
using namespace pyramid_scheme_simulator::gl;

int main(int argc, char** argv)
{
    std::cout << "Hello, GTK+!" << std::endl;
    const auto app = Gtk::Application::create(argc, argv);

    GLContext(Config::Defaults::defaultWindowOptions).run(app);
}
