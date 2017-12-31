#include <iostream>

#include "Config.hpp"
#include "gl/GLContext.hpp"

#include <gtkmm.h>

using namespace pyramid_scheme_simulator;
using namespace pyramid_scheme_simulator::gl;

int main(int argc, char** argv)
{
    std::cout << "Hello, GTK+!" << std::endl;

    GLContext(Config::Defaults::defaultWindowOptions).run(
            Gtk::Application::create(argc, argv));
}
