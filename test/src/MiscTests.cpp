#include <gtest/gtest.h>

#include <utility>
#include <unordered_map>
#include <string>

#include "Util/Unique.hpp"
#include "Util/Strcat.hpp"

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


}
