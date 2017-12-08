#pragma once

#include <spdlog/spdlog.h>

#include <memory>

#include "Config.hpp"
#include "Types.hpp"

namespace pyramid_scheme_simulator {

class TestConfig 
{
public:
    static const std::shared_ptr<spdlog::logger> logger;

    static const Config::ChanceDistribution tenPercentChanceDistribution;
    static const Config::ChanceDistribution randomChanceDistribution;
    static std::unique_ptr<Config> getBuildGraphConfig(rd_ptr);

    static const Money minStartingFunds, maxStartingFunds;
};

}
