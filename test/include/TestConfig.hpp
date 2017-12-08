#pragma once

#include "Config.hpp"
#include "Types.hpp"

namespace pyramid_scheme_simulator {

class TestConfig 
{
public:
    static const Config::ChanceDistribution tenPercentChanceDistribution;
    static const Config::ChanceDistribution randomChanceDistribution;
    static const std::unique_ptr<Config> getBuildGraphConfig(rd_ptr);

    static const Money minStartingFunds, maxStartingFunds;
};

}
