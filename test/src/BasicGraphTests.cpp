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


class BasicGraphTests : public ::testing::Test
{
public:
    Money startingMoney = 100;

    PopulationGraph::Pop distributor = 
        std::make_shared<EagerTestDistributor>(startingMoney);

    PopulationGraph::Pop customer1 = 
        std::make_shared<EagerTestConsumer>(startingMoney);

    PopulationGraph::Pop customer2 = 
        std::make_shared<EagerTestConsumer>(startingMoney);

    std::vector<std::pair<PopulationGraph::Pop, PopulationGraph::Pop>>
        tinyGraphTuples = {
                std::make_pair(distributor, customer1),
                std::make_pair(distributor, customer2),
                std::make_pair(customer1, customer2) };

    std::unique_ptr<PopulationGraph> tinyGraph = 
        make_unique<MockPopulationGraph>(tinyGraphTuples);

    std::unique_ptr<SaleHandler> saleHandler = 
        make_unique<MockSaleHandler>();

    rd_ptr rd = std::make_shared<std::mt19937_64>();


    const Money price = 10;
    const SimulationTick when = 0;
};

TEST_F(BasicGraphTests, BasicSaleTest)
{
    auto recordResult = (*saleHandler)(when, price, 
            rd, *distributor, *customer1);

    ASSERT_TRUE(recordResult.records.size() > 0);
}

TEST_F(BasicGraphTests, BasicNoInventoryTest)
{
    //sale shouldn't happen if there's no inventory
    distributor->setInventory(0);
    EXPECT_EQ(distributor->getInventory(), 0);

    auto recordResult = (*saleHandler)(when, price, 
            rd, *distributor, *customer1);

    ASSERT_EQ(recordResult.records.size(), 1);
    auto& rec = recordResult.records.front();

    EXPECT_TRUE(rec->isLeft());
    auto& res = rec->getLeft();
    ASSERT_EQ(res.reason, SalesResult::Reason::NO_INVENTORY) 
        << "Unexpected SalesResult: " << res.str();
}

TEST_F(BasicGraphTests, BasicRestockTest)
{
    //clear out the distributor's inventory
    distributor->setInventory(0);

    //get the restock list
    RestockHandler restockHandler;
    RestockHandler::RestockSet restockSet = 
        RestockHandler::toSet(
            std::move(restockHandler(when, price, *distributor)));

    SaleHandler h(std::move(restockSet));
    auto recordResult = h(when, price, rd, *distributor, *customer1);
}


}
