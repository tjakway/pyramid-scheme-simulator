#include <gtest/gtest.h>

#include "Util/Unique.hpp"

#include "mocks/MockPopulationGraph.hpp"

#include <unordered_set>

namespace pyramid_scheme_simulator {

using MockUndirectedEdge = 
    MockPopulationGraph::MockUndirectedEdge;

class UndirectedEdgeTests : public ::testing::Test
{
public:
    const Unique u = Unique::newUnique(),
                 v = Unique::newUnique();

    const MockUndirectedEdge uv = MockUndirectedEdge(u, v);
    const MockUndirectedEdge vu = MockUndirectedEdge(v, u);
};

TEST_F(UndirectedEdgeTests, TestEquality)
{
    ASSERT_EQ(uv, vu);
}

TEST_F(UndirectedEdgeTests, TestUnorderedSetMembership)
{
    std::unordered_set<MockUndirectedEdge> edgeSet;

    edgeSet.insert(uv);
    ASSERT_EQ(edgeSet.size(), 1);

    edgeSet.insert(vu);
    ASSERT_EQ(edgeSet.size(), 1);

    ASSERT_TRUE(*edgeSet.find(uv) == vu);
}

}
