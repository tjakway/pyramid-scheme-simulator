#include <gtest/gtest.h>

#include "Util/Util.hpp"
#include "TransactionRecords.hpp"
#include "TransactionObjects.hpp"
#include "PopulationGraph.hpp"
#include "Types.hpp"

#include "mocks/MockPopulationGraph.hpp"
#include "mocks/MockTransactionObjects.hpp"
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
    tinyGraphTuples = {
            std::make_pair(distributor, customer1),
            std::make_pair(distributor, customer2),
            std::make_pair(customer1, customer2) };

TEST(GraphTests, BasicTest)
{
    std::unique_ptr<PopulationGraph> tinyGraph = 
        make_unique<MockPopulationGraph>(tinyGraphTuples);

    std::unique_ptr<SaleHandler> saleHandler = 
        make_unique<MockSaleHandler>();

    rd_ptr rd = std::make_shared<std::mt19937_64>();

    const Money price = 10;
    const SimulationTick when = 0;

    auto recordResult = (*saleHandler)(when, price, rd, *distributor, *customer1);

    ASSERT_TRUE(recordResult.records.size() > 0);
}

}
