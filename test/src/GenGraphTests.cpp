#include <gtest/gtest.h>

#include "Config.hpp"
#include "Util/Util.hpp"
#include "Util/Unique.hpp"
#include "TransactionRecords.hpp"
#include "TransactionObjects.hpp"
#include "PopulationGraph.hpp"
#include "Types.hpp"
#include "CapitalHolderClassDecls.hpp"
#include "CapitalHolder.hpp"
#include "PopulationGraph.hpp"

#include "TestConfig.hpp"

namespace pyramid_scheme_simulator {

class GenGraphTests : public ::testing::Test
{
public:
    rd_ptr rd = std::make_shared<std::mt19937_64>();
};

TEST_F(GenGraphTests, BasicBuildGraphTest)
{
    const std::unique_ptr<Config> configPtr = TestConfig::getBuildGraphConfig(rd);
    PopulationGraph g(*configPtr);
}

}
