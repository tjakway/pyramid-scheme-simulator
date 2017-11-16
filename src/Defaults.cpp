#include "Defaults.hpp"
#include "Unique.hpp"
#include "Types.hpp"

namespace pyramid_scheme_simulator {

const double Defaults::defaultSalesChance = 0.1;
const double Defaults::defaultConversionChance = 0.1;

const std::function<std::shared_ptr<Consumer>(Unique, Money)> 
    Defaults::mkDefaultConsumer = 
        [](Unique u, Money m){
            return new StaticConsumer(
                    u, m,
                    defaultSalesChance,
                    defaultConversionChance
                    );
        };
}
