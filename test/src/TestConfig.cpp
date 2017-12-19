#include "TestConfig.hpp"
#include "Util/Util.hpp"
#include "ChanceContributor.hpp"

#include <spdlog/spdlog.h>

#include <limits>
#include <utility>

namespace pyramid_scheme_simulator {

const std::shared_ptr<spdlog::logger> TestConfig::logger = 
    spdlog::stdout_color_mt("test_console");

const Money TestConfig::minStartingFunds = 10;
const Money TestConfig::maxStartingFunds = 1000;

const Config::ChanceDistribution TestConfig::tenPercentChanceDistribution = 
    [](rd_ptr /*rd*/) -> std::unique_ptr<ChanceContributor> {
        return make_unique<StaticChanceContributor>(0.1);
    };


const Config::ChanceDistribution TestConfig::randomChanceDistribution = 
    [](rd_ptr /*rd*/) -> std::unique_ptr<ChanceContributor> {
        return make_unique<StaticChanceContributor>(0.1);
    };

std::unique_ptr<Config> TestConfig::getBuildGraphConfig(rd_ptr rd)
{
    return make_unique<Config>(
            make_unique<Config::SimulationOptions>(
                std::unique_ptr<Config::SimulationOptions::DistributorOptions>(),
                std::numeric_limits<SimulationTick>::max(), 
                //product cost of 1
                static_cast<Money>(1), static_cast<Money>(1),
                //sample starting funds from a random distribution
                [rd]() { 
                    return Util::sampleRdInRange(rd, 
                        std::make_pair(minStartingFunds, maxStartingFunds));
                    }
                ),
            make_unique<Config::GraphGenerationOptions>(
                false, //no disconnected subgraphs
                600, //graph size
                0.2, //link chance
                std::numeric_limits<unsigned long>::max(), //no limit on max edges per vertex
                tenPercentChanceDistribution, //sales chance
                tenPercentChanceDistribution, //conversion chance
                false //only initial onboarding
                )
            );
}

const double TestConfig::MarginsOfError::testLinkChance = 0.2;
}
