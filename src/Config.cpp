#include "Config.hpp"

#include <random>
#include <chrono>
#include <string>

#include "Types.hpp"

namespace {
    using namespace pyramid_scheme_simulator;
    rd_seed_type getCurrentTimeMillis()
    {
        return std::chrono::duration_cast< std::chrono::milliseconds >(
            std::chrono::system_clock::now().time_since_epoch()).count();
    }

    rd_ptr rdFromSeed(const rd_seed_type seed)
    {
        return std::make_shared<std::mt19937_64>(seed);
    }
}

namespace pyramid_scheme_simulator {

    Config(const rd_seed_type seed,
            
            ;
Config::Config(const rd_seed_type seed, 
    std::unique_ptr<SimulationOptions>&& simOptions,
    std::unique_ptr<GraphGenerationOptions>&& graphGenOptions)
    : randomSeed(seed), 
        simulationOptions(simOptions),
        graphGenerationOptions(graphGenOptions),
        //initialize the random generator from the passed seed
        randomGen(rdFromSeed(randomSeed))
{ }

//if no seed is given use current time
Config(std::unique_ptr<SimulationOptions>&& simOptions,
        std::unique_ptr<GraphGenerationOptions>&& graphGenOptions)
: Config(getCurrentTimeMillis(), simOptions, graphGenOptions)
{ }


Config::SimulationOptions::DistributorOptions::DistributorOptions(
        double pct, const unsigned int _buyIn, 
        Config::SimulationOptions::DistributorOptions::NewDistributorFunction f)
    : buyIn(_buyIn), newDistributorFunction(f)
{
    downstreamPercent.setOption(pct);
}


Config::SimulationOptions::SimulationOptions(
        std::unique_ptr<DistributorOptions>&& ops,
        const SimulationTick _maxTicks,
        const unsigned int _standardProductCost,
        const unsigned int _wholesaleProductCost,
        const std::function<Money()> _startingFunds)
    : distributionOptions(ops),
        maxTicks(_maxTicks),
        standardProductCost(_standardProductCost),
        wholesaleProductCost(_wholesaleProductCost),
        startingFunds(_startingFunds)
      {}

}
