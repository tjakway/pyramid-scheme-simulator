#include "Config.hpp"
#include "Unique.hpp"
#include "Types.hpp"
#include "Actors.hpp"

#include <memory>

namespace pyramid_scheme_simulator {

Config::Defaults::Defaults(Config* config) : config(config)
    {}


std::shared_ptr<Consumer> Config::Defaults::mkConsumer(rd_ptr rd, Unique u)
{
    const auto startingMoney = config->simulationOptions->startingFunds();
    //since we're making a StaticConsumer we only need 1 
    const auto salesChance = config->salesSkillDistribution(rd);
    const auto conversionChance = config->conversionChanceDistribution(rd);

    return std::make_shared<StaticConsumer>(u, startingMoney,
            salesChance, conversionChance);
}

}
