#include "Config.hpp"

#include <random>
#include <chrono>
#include <string>
#include <iostream>

#include "Util/Util.hpp"
#include "Types.hpp"
#include "CapitalHolderClassDecls.hpp"

namespace pyramid_scheme_simulator {

Config::Config(const rd_seed_type seed, 
    std::unique_ptr<SimulationOptions>&& simOptions,
    std::unique_ptr<GraphGenerationOptions>&& graphGenOptions)
    : randomSeed(seed), 
        //initialize the random generator from the passed seed
        randomGen(Util::rdFromSeed(randomSeed)),
        simulationOptions(std::move(simOptions)),
        graphGenerationOptions(std::move(graphGenOptions))
{ }

//if no seed is given use current time
Config::Config(std::unique_ptr<SimulationOptions>&& simOptions,
        std::unique_ptr<GraphGenerationOptions>&& graphGenOptions)
    : Config(Util::getCurrentTimeMillis(), 
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
    :  maxTicks(_maxTicks),
        standardProductCost(_standardProductCost),
        wholesaleProductCost(_wholesaleProductCost),
        startingFunds(_startingFunds),
        distributionOptions(std::move(ops))
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

const std::shared_ptr<spdlog::logger> Config::logger = [](){
    try {
        return spdlog::stdout_color_mt("pyramid_scheme_simulator");
    }
    catch (const spdlog::spdlog_ex& ex)
    {
        std::cerr << "Error while statically initializing Config: log init failed " 
            << ex.what() << std::endl;
        throw ex;
    }
}();

//arbitrary, tweak as needed
const Inventory Config::Defaults::defaultDesiredRestockAmount = 10;
const Inventory Config::Defaults::defaultRestockThreshold = 1;



}
