#include <gtest/gtest.h>

#include <utility>
#include <unordered_map>
#include <string>
#include <random>

#include "Util/Unique.hpp"
#include "Util/Util.hpp"

#include "TestConfig.hpp"


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

        const double average = numProcs / numSamples;

        ASSERT_TRUE(Util::withinMargins(average, sampleChance, TestConfig::allowedMarginOfError));
    }
}


namespace pyramid_scheme_simulator {

class Util_SampleFromTests : public ::testing::Test
{
public:
    rd_ptr rd = Util::rdSeededWithCurrentTime();
};

TEST_F(Util_SampleFromTests, TestGeneratedSampleChance)
{
    const double generatedSampleChance = std::uniform_int_distribution<unsigned int>{}(*rd) % 100;
    testSampleFrom(rd, generatedSampleChance, TestConfig::numSampleFromTests);
}

TEST_F(Util_SampleFromTests, TestZeroSampleChance)
{
    testSampleFrom(rd, 0.0, TestConfig::numSampleFromTests);
}

TEST_F(Util_SampleFromTests, TestOneSampleChance)
{
    testSampleFrom(rd, 1.0, TestConfig::numSampleFromTests);
}

TEST_F(Util_SampleFromTests, TestHalfSampleChance)
{
    testSampleFrom(rd, 0.5, TestConfig::numSampleFromTests);
}

TEST_F(Util_SampleFromTests, TestTwentyPctSampleChance)
{
    testSampleFrom(rd, 0.2, TestConfig::numSampleFromTests);
}

}
