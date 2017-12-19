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
    ASSERT_LE(configPtr->graphGenerationOptions->linkChance->getOption(), 1.0);

    MockPopulationGraph g(*configPtr);

    g.auditGraph();
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
                auto degreeOfVertex = boost::degree(v, *graphPtr);
                sumDegrees += degreeOfVertex;
                numVertices++;
            });
    g.auditGraph();

    ASSERT_GT(numVertices, 0);
    ASSERT_GT(sumDegrees, 
            static_cast<PopulationGraph::BGLPopulationGraph::degree_size_type>(0));

    ASSERT_EQ(numVertices, 
            static_cast<int>(configPtr->graphGenerationOptions->graphSize.getOption()));
    const double avgDegree = ((double)sumDegrees) / ((double)numVertices);


    //sanity checks that we didn't mess anything up while calculating degrees
    ASSERT_EQ(numVertices, boost::num_vertices(*graphPtr));
    ASSERT_EQ(numVertices, g.numVertices());

    const double dNumVertices = (double)numVertices;
    const double expectedAvgDegree = 
        (dNumVertices - 1) * configPtr->graphGenerationOptions->linkChance->getOption();

    const double allowedMarginOfError = TestConfig::MarginsOfError::testLinkChance / 2.0;
    ASSERT_LE(TestConfig::MarginsOfError::testLinkChance, 1.0);

    const double maxAllowedAvgDegree = 
        expectedAvgDegree * (1.0 + allowedMarginOfError);

    const double minAllowedAvgDegree =
        expectedAvgDegree * (1.0 - allowedMarginOfError);

    ASSERT_GE(avgDegree, minAllowedAvgDegree);
    ASSERT_LE(avgDegree, maxAllowedAvgDegree);

    g.auditGraph();
}

}
