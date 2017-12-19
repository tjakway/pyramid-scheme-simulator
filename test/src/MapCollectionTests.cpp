#include <gtest/gtest.h>

#include <cstdlib>
#include <algorithm>
#include <vector>
#include <set>
#include <unordered_set>
#include <deque>
#include <list>
#include <stdexcept>

#include "Util/Util.hpp"

namespace {
    int randBounded(int lowerBound = 1, int upperBound = 100)
    {
        assert(upperBound > lowerBound);
        return (rand() % (upperBound - lowerBound)) + lowerBound;
    }

    template <typename Collection>
    int genUniqueInt(Collection& col, int add)
    {
        int i = 0;
        while(true)
        {
            const int u = randBounded();
            //make sure that the collection of (element + add) is unique
            //i.e. that out call to mapCollection will be a bijection
            if(std::find(col.begin(), col.end(), u) == col.end()
                    && std::find(col.begin(), col.end(), u + add) == col.end())
            {
                return u;
            }

            if(i >= (col.size() * 100))
            {
                throw std::runtime_error("Too many iterations in genUniqueInt, probably an infinite loop");
            }
        }
    }

    template <typename Collection>
    void insertRandomElement(Collection& col, int /*add*/)
    {
        col.insert(col.end(), randBounded());
    }

    /**
     * sets delete nonunique values so we need to sample without replacement
     * from a random distribution to fill them properly
     */
    template <typename T>
    void fillContainer(std::set<T> col, int add)
    {
        genUniqueInt(col, add);
    }

    template <typename FromCollection, typename ToCollection>
    void testMapCollection(int add = randBounded())
    {
        //generate & fill the source collection
        FromCollection from;
        EXPECT_EQ(from.size(), 0);

        const int maxSize = 10;
        const int generatedSize = randBounded(1, maxSize);
        for(int i = 0; i < generatedSize; i++)
        {
            insertRandomElement(from, add);
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
        EXPECT_EQ(from.size(), to.size());

        typename FromCollection::iterator fIt = from.begin(),
                 fEnd = from.end();
        typename ToCollection::iterator tIt = to.begin(),
                 tEnd = to.end();

        while(fIt != fEnd)
        {
            //the mapping added some value
            ASSERT_EQ((*fIt) + add, *tIt);
            ++tIt;
            ++fIt;
        }

        ASSERT_TRUE(tIt == tEnd);
    }
}

namespace pyramid_scheme_simulator {

template <typename X>
void testXToAll(int param = randBounded())
{
    testMapCollection<X, std::vector<int>>(param);
    testMapCollection<X, std::list<int>>(param);
    testMapCollection<X, std::deque<int>>(param);
    testMapCollection<X, std::set<int>>(0);
}

//tests mapping a category to itself
TEST(MapColletionTests, testEndofunctors)
{
    testMapCollection<std::vector<int>, std::vector<int>>();
    testMapCollection<std::list<int>, std::list<int>>();
    testMapCollection<std::deque<int>, std::deque<int>>();
    testMapCollection<std::set<int>, std::set<int>>();
}

//should be able to map ordered collections when changing elements and still
//be able to test for equality
TEST(MapColletionTests, testOrdered)
{
    testXToAll<std::vector<int>>();
    testXToAll<std::list<int>>();
    testXToAll<std::deque<int>>();

    //have to pass 0 for set because it will sort itself
    testXToAll<std::set<int>>(0);
}


}
