#include <gtest/gtest.h>

#include <utility>
#include <unordered_map>
#include <string>
#include <random>

#include "Util/Unique.hpp"
#include "Util/Strcat.hpp"
#include "Util/Util.hpp"

#include "TestConfig.hpp"

namespace {
    using namespace pyramid_scheme_simulator;
    void testSampleFrom(rd_ptr rd, double sampleChance, unsigned int numSamples)
    {
        unsigned int numProcs = 0;

        for(unsigned int i = 0; i < numSamples; i++)
        {
            //true = 1, false = 0
            numProcs += Util::sampleFrom(rd, sampleChance);
        }

        const double average = numProcs / numSamples;

        ASSERT_TRUE(Util::withinMargins(average, sampleChance, TestConfig::allowedMarginOfError));
    }
}

namespace pyramid_scheme_simulator {

//sanity check that the hash<Unique> instance works
TEST(MiscTests, TestUnorderedMapOfUniques)
{
    std::unordered_map<Unique, Money> umap;
    umap.insert(std::make_pair(Unique(), 10));
    for(auto x : umap)
    {
        ASSERT_EQ(x.first.str(), "00000000-0000-0000-0000-000000000000");
        ASSERT_EQ(x.second, 10);
    }
}

TEST(MiscTests, TestSTRCAT)
{
    std::string barStr = "bar";
    ASSERT_EQ( STRCAT("foo", barStr), "foobar");

    ASSERT_EQ("610bazquz", STRCAT((int)6, (unsigned long)10, "baz", std::string("quz")));
}

namespace {
    std::string returnsOwnName(){
        return STRCAT(__func__);
    }
}
//STRCAT can be used with convenient debugging macros like __file__, __line__, etc.
TEST(MiscTests, TestSTRCATWithDebuggingMacros)
{
    //note: using __func__ in google tests gives unintuitive results because
    //gtest relies on macro magic to transform tests under the hood
    EXPECT_EQ(returnsOwnName(), "returnsOwnName");

    const std::string expected = "MiscTests.cpp";
    //__FILE__ can (always? sometimes? not sure) give an absolute path
    std::string path = STRCAT(__FILE__);
    ASSERT_TRUE(path.length() >= expected.length());

    auto rfindRes = path.rfind(expected);
    ASSERT_NE(rfindRes, std::string::npos);

    ASSERT_EQ(rfindRes + expected.length(), path.length());

}

class Util_SampleFromTests : public ::testing::Test
{
public:
    rd_ptr rd = std::make_shared<std::mt19937_64>();
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
