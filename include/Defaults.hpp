#pragma once

#include "CapitalHolder.hpp"
#include "Types.hpp"
#include "Unique.hpp"

#include <memory>
#include <functional>

namespace pyramid_scheme_simulator {

class Defaults
{
private:
    //static class
    Defaults() = delete;

    static const double defaultSalesChance;
    static const double defaultConversionChance;

public:
    /**
     * Consumer factory
     */
    static const std::function<std::shared_ptr<Consumer>(Unique, Money)> 
        mkDefaultConsumer;
};

}
