#include "Config.hpp"
#include "Util/Unique.hpp"
#include "Types.hpp"
#include "Actors.hpp"

#include <memory>

namespace pyramid_scheme_simulator {

Config::Defaults::Defaults(Config* config) : config(config)
    {}


std::shared_ptr<Consumer> Config::Defaults::mkConsumer(rd_ptr rd, Unique u)
{
    auto startingMoney = config->simulationOptions->startingFunds();
    //since we're making a StaticConsumer we only need 1 
    auto salesChance = 
        config->graphGenerationOptions->salesSkillDistribution(rd);
    auto conversionChance = 
        config->graphGenerationOptions->conversionChanceDistribution(rd);

    return std::make_shared<StaticConsumer>(u, startingMoney,
            salesChance, conversionChance);
}

}
