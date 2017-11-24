#include <gtest/gtest.h>

#include "TransactionRecords.hpp"
#include "TransactionObjects.hpp"
#include "PopulationGraph.hpp"
#include "Types.hpp"

#include "mocks/TestPopulationGraph.hpp"
#include "mocks/EagerActors.hpp"

#include <vector>
#include <utility>
#include <tuple>
#include <memory>

namespace pyramid_scheme_simulator {

static Money startingMoney = 100;

static PopulationGraph::Pop distributor = 
    std::make_shared<EagerTestDistributor>(startingMoney);

static PopulationGraph::Pop customer1 = 
    std::make_shared<EagerTestConsumer>(startingMoney);

static PopulationGraph::Pop customer2 = 
    std::make_shared<EagerTestConsumer>(startingMoney);

static std::vector<std::pair<PopulationGraph::Pop, PopulationGraph::Pop>>
    tinyGraph = {
            std::make_pair(distributor, customer1),
            std::make_pair(distributor, customer2),
            std::make_pair(customer1, customer2) };

TEST(GraphTests, BasicTests)
{
}

}
