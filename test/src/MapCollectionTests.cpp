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

#define RAND_BOUNDED_LOWER 1
#define RAND_BOUNDED_UPPER 100

namespace {
    int randBounded(int lowerBound = RAND_BOUNDED_LOWER, 
            int upperBound = RAND_BOUNDED_UPPER)
    {
        assert(upperBound > lowerBound);
        const int gen = (rand() % (upperBound - lowerBound)) + lowerBound;
        if(gen > upperBound)
        {
            return upperBound;
        }
        else
        {
            return gen;
        }
    }

    template <typename Collection>
    int genUniqueInt(Collection& col, int add, 
            int lowerBound = RAND_BOUNDED_LOWER,
            int upperBound = RAND_BOUNDED_UPPER)
    {
        unsigned long i = 0;
        while(true)
        {
            const int u = randBounded(lowerBound, upperBound);
            //make sure that the collection of (element + add) is unique
            //i.e. that out call to mapCollection will be a bijection
            if(std::find(col.begin(), col.end(), u) == col.end()
                    && std::find(col.begin(), col.end(), u + add) == col.end())
            {
                return u;
            }

            if(i >= (col.size() * 100))
            {
                throw std::runtime_error("Too many iterations "
                         "in genUniqueInt, probably an infinite loop");
            }
        }
    }

    template <typename Collection>
    void insertRandomElement(Collection& col, int add)
    {
        col.insert(col.end(), genUniqueInt(col, add));
    }

    /**
     * sets delete nonunique values so we need to sample without replacement
     * from a random distribution to fill them properly
     */
    void insertRandomElement(std::set<int>& col, int add)
    {
        col.emplace(genUniqueInt(col, add));
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

        //convert everything to ordered containers before comparing
        std::vector<int> cmpTo(to.begin(), to.end()),
            cmpFrom(from.begin(), from.end());

        std::sort(cmpTo.begin(), cmpTo.end());
        std::sort(cmpFrom.begin(), cmpFrom.end());

        typename std::vector<int>::iterator fIt = cmpFrom.begin(),
                 fEnd = cmpFrom.end();
        typename std::vector<int>::iterator tIt = cmpTo.begin(),
                 tEnd = cmpTo.end();

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
    //have to pass 0 for set because it will sort itself
    testMapCollection<X, std::set<int>>(0);
}

//need to have different behavior for set and unordered_set,
//which don't have an emplace method that takes an iterator
//(they use emplace_hint instead)
template <typename T>
class TestGenUniqueEmplacer
{
public:
    void doEmplace(T& collection, int i)
    {
        collection.emplace(collection.end(), i);
    }
};

template <>
class TestGenUniqueEmplacer<std::set<int>>
{
public:
    void doEmplace(std::set<int>& collection, int i)
    {
        collection.emplace_hint(collection.end(), i);
    }
};

template <>
class TestGenUniqueEmplacer<std::unordered_set<int>>
{
public:
    void doEmplace(std::unordered_set<int>& collection, int i)
    {
        collection.emplace_hint(collection.end(), i);
    }
};

//internal test
//test that filling a container with genUniqueInt 
//works as expected
template <typename Collection>
void testGenUnique(std::function<void(Collection&)> sortF, 
        int lowerBound = 1, 
        int upperBound = 100)
{
    Collection uniformCol,
        generatedCol;

    unsigned long numIterations = 0;
    for(int i = lowerBound; i < upperBound; i++)
    {
        TestGenUniqueEmplacer<Collection>().doEmplace(uniformCol, i);

        const int genElem = genUniqueInt(generatedCol, 0, lowerBound, upperBound);
        TestGenUniqueEmplacer<Collection>().doEmplace(generatedCol, genElem);

        numIterations++;
    }

    EXPECT_EQ(uniformCol.size(), numIterations);
    EXPECT_EQ(uniformCol.size(), generatedCol.size());

    //sorting the uniform collection shouldn't do anything
    Collection uniformColPreSort = uniformCol;
    sortF(uniformCol);
    EXPECT_EQ(uniformColPreSort, uniformCol);

    sortF(generatedCol);
    ASSERT_EQ(uniformCol, generatedCol);
}

//had to refactor out container sorting code because
//std::sort doesn't work for std::list (std::sort
//needs a random access iterator)
template <typename T>
class Sorter
{
public:
    const std::function<void(T&)> sortF =
        [](T& col)
        {
            std::sort(col.begin(), col.end());
        };
};

template<>
class Sorter<std::list<int>>
{
public:
    const std::function<void(std::list<int>&)> sortF =
        [](std::list<int>& s)
        {
            s.sort();
        };
};

TEST(MapCollectionTests, testGenUniqueSequenceContainers)
{
    testGenUnique<std::vector<int>>(Sorter<std::vector<int>>().sortF);
    testGenUnique<std::list<int>>(Sorter<std::list<int>>().sortF);
    testGenUnique<std::deque<int>>(Sorter<std::deque<int>>().sortF);
}

TEST(MapCollectionTests, testGenUniqueSet)
{
    //can't sort a set
    const std::function<void(std::set<int>&)> set_no_op = [](std::set<int>&){};
    testGenUnique<std::set<int>>(set_no_op);

    const std::function<void(std::unordered_set<int>&)> unordered_set_no_op = 
        [](std::unordered_set<int>&){};
    testGenUnique<std::unordered_set<int>>(unordered_set_no_op);
}

//tests mapping a category to itself
TEST(MapCollectionTests, testEndofunctors)
{
    testMapCollection<std::vector<int>, std::vector<int>>();
    testMapCollection<std::list<int>, std::list<int>>();
    testMapCollection<std::deque<int>, std::deque<int>>();
    testMapCollection<std::set<int>, std::set<int>>();
    testMapCollection<std::unordered_set<int>, std::unordered_set<int>>();
}

//should be able to map ordered collections when changing elements and still
//be able to test for equality
TEST(MapCollectionTests, testXToAllCollections)
{
    testXToAll<std::vector<int>>();
    testXToAll<std::list<int>>();
    testXToAll<std::deque<int>>();

    //have to pass 0 for set because it will sort itself
    testXToAll<std::set<int>>(0);
    testXToAll<std::unordered_set<int>>(0);
}


}
