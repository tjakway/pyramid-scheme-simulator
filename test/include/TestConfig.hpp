#pragma once

#include <spdlog/spdlog.h>

#include <memory>

#include "Config.hpp"
#include "Types.hpp"

namespace pyramid_scheme_simulator {

class TestConfig 
{
protected:
    TestConfig() = delete;
public:
    static const std::shared_ptr<spdlog::logger> logger;

    static const Config::ChanceDistribution tenPercentChanceDistribution;
    static const Config::ChanceDistribution randomChanceDistribution;
    static std::unique_ptr<Config> getBuildGraphConfig(rd_ptr);

    static const Money minStartingFunds, maxStartingFunds;

    /**
     * allowed margins of error (percent)
     * each double is the total value, i.e. plus or minus (allowedMarginOfError / 2)
     */
    class MarginsOfError
    {
    protected:
        MarginsOfError() = delete;
    public:
        static constexpr double defaultMargin = 0.1;

        /**
         * widen the margin of error for GenGraphTests::testLinkChance
         * because increasing the sample size to bring the error within the default
         * margin makes the test prohibitively slow
         */
        static const double testLinkChance;
    };

    static const unsigned int numSampleFromTests = 6000;
};

}
