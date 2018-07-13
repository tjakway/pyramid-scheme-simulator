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
    //make sure the consumer in our test has enough cash to afford the buy in
    ASSERT_GT(startingMoney, buyIn);


}

END_PYRAMID_NAMESPACE
