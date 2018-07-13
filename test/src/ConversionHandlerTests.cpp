#include <gtest/gtest.h>

#include "NamespaceDefines.hpp"

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

BEGIN_PYRAMID_NAMESPACE

using ConversionHandlerTests = BasicGraphSetup;

/**
 * run the ConversionHandler against our pairs and make sure they work out how we expect
 * TODO: this test shares too much code with Simulation::applyConversions, DRY
 */
TEST_F(ConversionHandlerTests, BasicConversionRecordTest)
{
    const Money buyIn = 1;

    //make sure the consumer in our test has enough cash to afford the buy in
    ASSERT_GT(buyIn, startingMoney);

    ConversionHandler conversionHandler(rd_ptr, 1);

    conversionHandler(consumer1, distributor);

    //curry it with the SimulationTick
    //see note in Simulation.cpp: TODO: consider refactoring into another top-level class that can be derived by test classes
    const auto f = [this](std::pair<PopulationGraph::Pop, PopulationGraph::Pop> pops) {
        return this->conversionHandler.operator()(this->when(), pops)
    };

    std::vector<ConversionHandler::RecordType> vecConversions = 
        populationGraph->forEachEdge(f);

    auto conversionRecs = 
        emptyListTransactionRecord<ConversionHandler::ElementType>().leftFold(
                std::list<ConversionHandler::RecordType>(
                    std::make_move_iterator(vecConversions.begin()),
                    std::make_move_iterator(vecConversions.end())),
                conversionHandler.reduce);


    const auto numConversions = processConversions(conversionRecs);
    assert(numConversions == conversionRecs.records.size());

}

END_PYRAMID_NAMESPACE
