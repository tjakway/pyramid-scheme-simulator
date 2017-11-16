#pragma once

#include "CapitalHolder.hpp"

#include <memory>
#include <functional>

namespace pyramid_scheme_simulator {

class Defaults
{
private:
    //static class
    Defaults() = delete;

public:
    static const std::function<std::shared_ptr<Consumer>()>> mkDefaultConsumer;
};

}
