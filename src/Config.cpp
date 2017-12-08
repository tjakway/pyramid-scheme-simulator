#include "Config.hpp"

#include <random>
#include <chrono>
#include <string>

#include "Util/Util.hpp"
#include "Types.hpp"
#include "CapitalHolderClassDecls.hpp"

namespace {
    using namespace pyramid_scheme_simulator;
    rd_seed_type getCurrentTimeMillis()
    {
        return static_cast<rd_seed_type>(std::chrono::duration_cast< 
                std::chrono::milliseconds >(
            std::chrono::system_clock::now().time_since_epoch()).count());
    }

    rd_ptr rdFromSeed(const rd_seed_type seed)
    {
        return std::make_shared<std::mt19937_64>(seed);
    }
}

namespace pyramid_scheme_simulator {

Config::Config(const rd_seed_type seed, 
    std::unique_ptr<SimulationOptions>&& simOptions,
    std::unique_ptr<GraphGenerationOptions>&& graphGenOptions)
    : randomSeed(seed), 
        simulationOptions(std::move(simOptions)),
        graphGenerationOptions(std::move(graphGenOptions)),
        //initialize the random generator from the passed seed
        randomGen(rdFromSeed(randomSeed))
{ }

//if no seed is given use current time
Config::Config(std::unique_ptr<SimulationOptions>&& simOptions,
        std::unique_ptr<GraphGenerationOptions>&& graphGenOptions)
    : Config(getCurrentTimeMillis(), 
        std::move(simOptions), 
        std::move(graphGenOptions))
{ }


Config::SimulationOptions::DistributorOptions::DistributorOptions(
        double pct, const unsigned int _buyIn, 
        NewDistributorFunction f)
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
    : distributionOptions(std::move(ops)),
        maxTicks(_maxTicks),
        standardProductCost(_standardProductCost),
        wholesaleProductCost(_wholesaleProductCost),
        startingFunds(_startingFunds)
      {}

Config::GraphGenerationOptions::GraphGenerationOptions(
        const bool _allowDisconnectedSubgraphs,
        const unsigned long _graphSize,
        const double _linkChance,
        const unsigned long _maxEdgesPerVertex,
        const Config::ChanceDistribution _salesSkillDistribution,
        const Config::ChanceDistribution _conversionChanceDistribution,
        const bool _onlyInitialOnboarding)
    : allowDisconnectedSubgraphs(_allowDisconnectedSubgraphs),
    linkChance(make_unique<PercentOption>(_linkChance)),
    salesSkillDistribution(_salesSkillDistribution),
    conversionChanceDistribution(_conversionChanceDistribution),
    onlyInitialOnboarding(_onlyInitialOnboarding)
{
    graphSize.setOption(_graphSize);
    maxEdgesPerVertex.setOption(_maxEdgesPerVertex);
}

}
