#include "Defaults.hpp"
#include "Unique.hpp"
#include "Types.hpp"

namespace pyramid_scheme_simulator {

const std::function<std::shared_ptr<Consumer>(Unique, Money)> 
    Defaults::mkDefaultConsumer = 
        [](Unique u, Money m){
            StaticConsumer()
        };
}
