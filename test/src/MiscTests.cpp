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


}
