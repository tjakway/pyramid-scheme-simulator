#include <gtest/gtest.h>

#include <algorithm>
#include <cstdlib>

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
            from.insert(rand());
        }

        EXPECT_EQ(from.size(), generatedSize);

        //some random mapping function
        std::function<typename ToCollection::value_type(typename FromCollection::value_type)>
            mapping = 
            [](typename FromCollection::value_type x) -> typename ToCollection::value_Type
            {
                return x + 1;
            };

        ToCollection to = 
            pyramid_scheme_simulator::Util::mapCollection<FromCollection, 
                                                        ToCollection>(from, mapping);

        EXPECT_EQ(to.size(), generatedSize);
        ASSERT_TRUE(std::equal(from.begin(), from.end(), to.begin(), to.end()));
    }
}
