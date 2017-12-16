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
    void testMapCollection()
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
            [](typename FromCollection::value_type x) -> typename ToCollection::value_type
            {
                return x + 1;
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
            ASSERT_EQ((*fIt) + 1, *tIt);
            ++tIt;
            ++fIt;
        }

        ASSERT_EQ(tIt, tEnd);
    }
}

namespace pyramid_scheme_simulator {

TEST(UtilTests, testMapCollectionVectorToVector)
{
    testMapCollection<std::vector<int>, std::vector<int>>();
}

}
