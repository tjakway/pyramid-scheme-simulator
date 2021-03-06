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

#include "BasicGraphSetup.hpp"

#include <unordered_map>
#include <functional>
#include <vector>
#include <utility>
#include <tuple>
#include <memory>
#include <random>
#include <iostream>

namespace pyramid_scheme_simulator {

using BasicGraphTests = BasicGraphSetup;

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
    EXPECT_EQ(distributor->getInventory(), static_cast<Inventory>(0));

    auto recordResult = (*saleHandler)(when, price, 
            rd, *distributor, *consumer1);

    ASSERT_EQ(recordResult.records.size(), 1L);
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
            restockHandler(when, price, *distributor));

    SaleHandler h(std::move(restockSet));
    auto recordResult = h(when, price, rd, *distributor, *consumer1);
}

TEST_F(BasicGraphTests, TestVertices)
{
    auto verts = tinyGraph->vertices();
    const auto numVerts = tinyGraph->numVertices();

    ASSERT_EQ(numVerts, verts.size());
}

TEST_F(BasicGraphTests, MutateVertices_MoneyTest)
{
    //change everyone's money and see if it sticks
    const unsigned long numVerticesPrev = tinyGraph->numVertices();
    
    std::unordered_map<Unique, Money> newMoneyMap;

    std::mt19937_64 rd;

    auto mutateFunction = [&newMoneyMap, &rd](const std::shared_ptr<CapitalHolder> h)
    {
        //assign them some random value
        const Money newMoney = rd() % 1000;
        newMoneyMap.insert(std::make_pair(h->id, newMoney));
        h->setMoney(newMoney);

        //not replacing the vertex
        return h;
    };
    
    EXPECT_EQ(numVerticesPrev, tinyGraph->numVertices());
    tinyGraph->mutateVertices(mutateFunction);
    EXPECT_EQ(numVerticesPrev, tinyGraph->numVertices());

    //check that the changes persist
    for(auto x : tinyGraph->vertices())
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
                convertedBy);

    tinyGraph->mutateVerticesWithPredicate(
            [newDistributor](const PopulationGraph::Pop /*popPtr*/) {
                //replace the consumer at that vertex 
                //with a distributor constructed from the consumer's data
                return newDistributor;
            },
            //don't forget that the argument to the predicate has to be const
            [this](const CapitalHolder& thisH) -> bool {
                return consumer1->operator==(thisH);
            });

    //make sure it worked
    ASSERT_EQ(newDistributor->id, consumer1->id);

    //our stored consumer pointer shouldn't be freed
    ASSERT_EQ(std::dynamic_pointer_cast<Distributor>(consumer1).get(), nullptr);

    //check the graph's shared_ptr
    CapitalHolder& vertRef = tinyGraph->findVertexByUnique(newDistributor->id);
    ASSERT_EQ(vertRef.id, newDistributor->id);
    ASSERT_EQ(vertRef.id, consumer1->id);


    ASSERT_TRUE(vertRef.isDistributor());
    //verify the underlying type
    ASSERT_NE(dynamic_cast<Distributor*>(&vertRef), nullptr);

}
}
