#include <gtest/gtest.h>

#include <utility>
#include <unordered_map>
#include <string>
#include <random>

#include "Util/Unique.hpp"
#include "Util/Util.hpp"

#include "TestConfig.hpp"

/**
 * tests of the Util::sampleFrom method
 */

namespace {
    using namespace pyramid_scheme_simulator;
    void testSampleFrom(rd_ptr rd, double sampleChance, unsigned int numSamples)
    {
        unsigned int numProcs = 0;

        for(unsigned int i = 0; i < numSamples; i++)
        {
            if(Util::sampleFrom(rd, sampleChance) == true)
            {
                numProcs++;
            }
        }

        const double average = ((double)numProcs) / ((double)numSamples);

        ASSERT_TRUE(Util::withinMargins(average, sampleChance, TestConfig::allowedMarginOfError));
    }
}


namespace pyramid_scheme_simulator {

class SampleFromTests : public ::testing::Test
{
public:
    rd_ptr rd = Util::rdSeededWithCurrentTime();
};

TEST_F(SampleFromTests, TestGeneratedSampleChance)
{
    const double generatedSampleChance = 
        (std::uniform_int_distribution<unsigned int>{}(*rd) % 100) / 100;
    testSampleFrom(rd, generatedSampleChance, TestConfig::numSampleFromTests);
}

TEST_F(SampleFromTests, TestZeroSampleChance)
{
    testSampleFrom(rd, 0.0, TestConfig::numSampleFromTests);
}

TEST_F(SampleFromTests, TestOneSampleChance)
{
    testSampleFrom(rd, 1.0, TestConfig::numSampleFromTests);
}

TEST_F(SampleFromTests, TestHalfSampleChance)
{
    testSampleFrom(rd, 0.5, TestConfig::numSampleFromTests);
}

TEST_F(SampleFromTests, TestTwentyPctSampleChance)
{
    testSampleFrom(rd, 0.2, TestConfig::numSampleFromTests);
}

}
