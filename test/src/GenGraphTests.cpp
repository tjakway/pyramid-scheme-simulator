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

#include "mocks/MockPopulationGraph.hpp"
#include "TestConfig.hpp"

#include <gtest/gtest.h>
#include <algorithm>

namespace pyramid_scheme_simulator {

class GenGraphTests : public ::testing::Test
{
public:
    rd_ptr rd = std::make_shared<std::mt19937_64>();
};

TEST_F(GenGraphTests, TestLinkChance)
{
    const std::unique_ptr<Config> configPtr = TestConfig::getBuildGraphConfig(rd);
    MockPopulationGraph g(*configPtr);

    auto graphPtr = g.getGraphPtr();

    PopulationGraph::BGLPopulationGraph::vertex_iterator vi, viEnd;
    std::tie(vi, viEnd) = boost::vertices(*graphPtr);

    //average the degrees and check that they match what we would expect based
    //on the link chance
    int numVertices = 0;
    PopulationGraph::BGLPopulationGraph::degree_size_type sumDegrees = 0;

    //can't use PopulationGraph::forEachVertex because we need to pass the vertex_descriptor
    //to boost::degree, which requires access to the internal graph structure
    std::for_each(vi, viEnd, 
            [&numVertices, &sumDegrees, graphPtr]
            (PopulationGraph::BGLPopulationGraph::vertex_descriptor v) {
                sumDegrees += boost::degree(v, *graphPtr);
                numVertices++;
            });

    ASSERT_GT(numVertices, 0);
    ASSERT_GT(sumDegrees, 0);

    ASSERT_EQ(numVertices, configPtr->graphGenerationOptions->graphSize.getOption());
    const double avgDegree = ((double)sumDegrees) / ((double)numVertices);


    const auto gNumVertices = boost::num_vertices(*graphPtr);
    
    //sanity checks that we didn't mess anything up while calculating degrees
    ASSERT_EQ(numVertices, gNumVertices);
    ASSERT_EQ(numVertices, g.numVertices());

    const double expectedAvgDegree = ((double)numVertices) * 
        configPtr->graphGenerationOptions->linkChance->getOption();

    const double maxAllowedAvgDegree = 
        expectedAvgDegree + (TestConfig::allowedMarginOfError / 2);

    const double minAllowedAvgDegree =
        expectedAvgDegree - (TestConfig::allowedMarginOfError / 2);

    ASSERT_GE(avgDegree, minAllowedAvgDegree);
    ASSERT_LE(avgDegree, maxAllowedAvgDegree);
}

}
