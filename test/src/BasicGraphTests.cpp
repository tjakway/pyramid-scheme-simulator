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

#include "mocks/MockPopulationGraph.hpp"
#include "mocks/MockTransactionObjects.hpp"
#include "mocks/EagerActors.hpp"

#include <unordered_map>
#include <functional>
#include <vector>
#include <utility>
#include <tuple>
#include <memory>
#include <random>
#include <iostream>

namespace pyramid_scheme_simulator {


class BasicGraphTests : public ::testing::Test
{
public:
    Money startingMoney = 100;

    PopulationGraph::Pop distributor = 
        std::make_shared<EagerTestDistributor>(startingMoney);

    PopulationGraph::Pop consumer1 = 
        std::make_shared<EagerTestConsumer>(startingMoney);

    PopulationGraph::Pop consumer2 = 
        std::make_shared<EagerTestConsumer>(startingMoney);

    std::vector<std::pair<PopulationGraph::Pop, PopulationGraph::Pop>>
        tinyGraphTuples = {
                std::make_pair(distributor, consumer1),
                std::make_pair(distributor, consumer2),
                std::make_pair(consumer1, consumer2) };

    std::unique_ptr<PopulationGraph> tinyGraph = 
        make_unique<MockPopulationGraph>(tinyGraphTuples);

    std::unique_ptr<SaleHandler> saleHandler = 
        make_unique<MockSaleHandler>();

    rd_ptr rd = std::make_shared<std::mt19937_64>();

    std::vector<PopulationGraph::Pop> getAllPops()
    {
        return {
            distributor,
            consumer1,
            consumer2
        };
    }

    const Money price = 10;
    const SimulationTick when = 0;
};

TEST_F(BasicGraphTests, BasicSaleTest)
{
    auto recordResult = (*saleHandler)(when, price, 
            rd, *distributor, *consumer1);

    ASSERT_TRUE(recordResult.records.size() > 0);
}

TEST_F(BasicGraphTests, BasicNoInventoryTest)
{
    //sale shouldn't happen if there's no inventory
    distributor->setInventory(0);
    EXPECT_EQ(distributor->getInventory(), 0);

    auto recordResult = (*saleHandler)(when, price, 
            rd, *distributor, *consumer1);

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
    auto recordResult = h(when, price, rd, *distributor, *consumer1);
}

TEST_F(BasicGraphTests, MutateVertices_MoneyTest)
{
    //change everyone's money and see if it sticks
    const int numVerticesPrev = tinyGraph->numVertices();
    
    std::unordered_map<Unique, Money> newMoneyMap;

    rd_ptr rd = std::make_shared<std::mt19937_64>();

    auto mutateFunction = [&newMoneyMap, &rd](std::shared_ptr<CapitalHolder>& h)
    {
        //assign them some random value
        const Money newMoney = (*rd)();
        newMoneyMap.insert(std::make_pair(h->id, newMoney));
        h->setMoney(newMoney);
    };
    
    EXPECT_EQ(numVerticesPrev, tinyGraph->numVertices());
    tinyGraph->mutateVertices(mutateFunction);


    //check that the changes persist
    for(auto x : getAllPops())
    {
        ASSERT_EQ(newMoneyMap[x->id], x->getMoney());
    }
}

TEST_F(BasicGraphTests, BecomeDistributorTest)
{
    NewDistributorFunction newDistributorFunction = 
        [](Consumer& who, std::shared_ptr<Distributor> convertedBy)
        {
            return std::make_shared<EagerTestDistributor>(who, convertedBy);
        };

    std::shared_ptr<Distributor> convertedBy = 
        std::dynamic_pointer_cast<Distributor>(distributor);

    PopulationGraph::Pop newDistributor = 
        std::dynamic_pointer_cast<Consumer>(consumer1)->becomeDistributor(
                newDistributorFunction,
                convertedBy);

    tinyGraph->mutateVerticesWithPredicate(
            [&newDistributor](PopulationGraph::Pop& popPtr) -> void {
                //replace the consumer at that vertex 
                //with a distributor constructed from the consumer's data
                popPtr = newDistributor;
            },
            //don't forget that the argument to the predicate has to be const
            [this](const CapitalHolder& thisH) -> bool {
                return consumer1->operator==(thisH);
            });

    //make sure it worked
    ASSERT_EQ(newDistributor->id, consumer1->id);

    //our stored consumer pointer shouldn't be freed
    ASSERT_EQ(std::dynamic_pointer_cast<Distributor>(consumer1).get(), nullptr);
}


}
