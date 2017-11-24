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

Config::Config(const rd_seed_type seed, const Config::SimulationOptions simOptions,
    const Config::GraphGenerationOptions graphGenOptions)

    : randomSeed(seed), simulationOptions(simOptions),
        graphGenerationOptions(graphGenOptions),
        //initialize the random generator from the passed seed
        randomGen(rdFromSeed(randomSeed))
{ }

//if no seed is given use current time
Config::Config(const Config::SimulationOptions simOptions,
    const Config::GraphGenerationOptions graphGenOptions)
    : Config(getCurrentTimeMillis(), simOptions, graphGenOptions)
{ }



}
