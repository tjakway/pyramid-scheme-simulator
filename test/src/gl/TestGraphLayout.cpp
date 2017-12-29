#include <gtest/gtest.h>

#include "gl/GraphLayout.hpp"

#include "BasicGraphSetup.hpp"


namespace pyramid_scheme_simulator {

using BasicGraphLayoutTests = BasicGraphSetup;

TEST_F(BasicGraphLayoutTests, TestCTOR)
{
    GraphLayout {Config::Defaults::defaultGraphLayoutOptions,
        *tinyGraph };
}

TEST_F(BasicGraphLayoutTests, TestCalculateLayout)
{
    GraphLayout graphLayout(
            Config::Defaults::defaultGraphLayoutOptions,
            *tinyGraph);

    const auto ret = graphLayout.calculateLayout();

    EXPECT_FALSE(ret.first.get() == nullptr);

    GraphLayout::printGraphLayout(std::cout, *ret.first);
}

}
