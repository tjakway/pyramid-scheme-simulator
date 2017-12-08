#include "Config.hpp"
#include "Util/Unique.hpp"
#include "Types.hpp"
#include "Actors.hpp"

#include <memory>

namespace pyramid_scheme_simulator {

Config::Defaults::Defaults()
    {}


std::shared_ptr<Consumer> Config::Defaults::mkConsumer(Config& conf, rd_ptr rd, Unique u)
{
    auto startingMoney = conf.simulationOptions->startingFunds();
    //since we're making a StaticConsumer we only need 1 
    auto salesChance = 
        conf.graphGenerationOptions->salesSkillDistribution(rd);
    auto conversionChance = 
        conf.graphGenerationOptions->conversionChanceDistribution(rd);

    return std::make_shared<StaticConsumer>(u, startingMoney,
            salesChance, conversionChance);
}

}
