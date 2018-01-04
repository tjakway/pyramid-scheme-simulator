#include <iostream>

#include "Config.hpp"
#include "gl/GLContext.hpp"

using namespace pyramid_scheme_simulator;
using namespace pyramid_scheme_simulator::gl;

int main(int argc, char** argv)
{
    std::cout << "Hello, SDL2!" << std::endl;

    GLContext(Config::Defaults::defaultWindowOptions).run();
}
