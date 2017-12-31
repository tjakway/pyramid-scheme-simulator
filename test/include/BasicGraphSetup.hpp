#pragma once

#include <gtest/gtest.h>

#include "Config.hpp"
#include "Util/Util.hpp"
#include "TransactionObjects.hpp"
#include "PopulationGraph.hpp"
#include "Types.hpp"
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

class BasicGraphSetup : public ::testing::Test
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

    const Money price = 10;
    const SimulationTick when = 0;
};

}
