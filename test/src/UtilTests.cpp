#include <gtest/gtest.h>

#include <cstdlib>
#include <algorithm>
#include <vector>
#include <set>
#include <unordered_set>
#include <deque>
#include <list>

#include "Util/Util.hpp"

namespace {
    template <typename FromCollection, typename ToCollection>
    void testMapCollection(int add = (rand() % 100))
    {
        //generate & fill the source collection
        FromCollection from;
        const int generatedSize = (rand() % 100) + 1;
        for(int i = 0; i < generatedSize; i++)
        {
            from.insert(from.end(), rand());
        }

        EXPECT_EQ(from.size(), generatedSize);

        //some random mapping function
        std::function<typename ToCollection::value_type(typename FromCollection::value_type)>
            mapping = 
            [add](typename FromCollection::value_type x) -> typename ToCollection::value_type
            {
                return x + add;
            };

        ToCollection to = 
            pyramid_scheme_simulator::Util::mapCollection<FromCollection, 
                                                        ToCollection>(from, mapping);

        EXPECT_EQ(to.size(), generatedSize);

        typename FromCollection::iterator fIt = from.begin(),
                 fEnd = from.end();
        typename ToCollection::iterator tIt = to.begin(),
                 tEnd = to.end();

        while(fIt != fEnd)
        {
            //the mapping added 1
            ASSERT_EQ((*fIt) + add, *tIt);
            ++tIt;
            ++fIt;
        }

        ASSERT_TRUE(tIt == tEnd);
    }
}

namespace pyramid_scheme_simulator {

template <typename X>
void testXToAll(int param = (rand() % 100))
{
    testMapCollection<X, std::vector<int>>(param);
    testMapCollection<X, std::list<int>>(param);
    testMapCollection<X, std::deque<int>>(param);
    testMapCollection<X, std::set<int>>(0);
}

//tests mapping a category to itself
TEST(UtilTests, testMapCollectionEndofunctors)
{
    testMapCollection<std::vector<int>, std::vector<int>>();
    testMapCollection<std::list<int>, std::list<int>>();
    testMapCollection<std::deque<int>, std::deque<int>>();
    testMapCollection<std::set<int>, std::set<int>>();
}

//should be able to map ordered collections when changing elements and still
//be able to test for equality
TEST(UtilTests, testMapCollectionOrdered)
{
    testXToAll<std::vector<int>>();
    testXToAll<std::list<int>>();
    testXToAll<std::deque<int>>();

    //have to pass 0 for set because it will sort itself
    testXToAll<std::set<int>>(0);
}


}
