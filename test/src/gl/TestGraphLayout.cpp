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

}
