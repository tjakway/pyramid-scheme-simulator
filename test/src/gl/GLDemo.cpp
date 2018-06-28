#include "gl/GLBackend.hpp"
#include "PopulationGraph.hpp"
#include "TestConfig.hpp"

#include "BasicGraphSetup.hpp"

namespace pyramid_scheme_simulator {

TEST_F(BasicGraphSetup, gldemo)
{
    gl::GLBackend backend(
            Config::Defaults::defaultGraphLayoutOptions,
            Config::Defaults::defaultWindowOptions);

    backend.exportData(
            std::make_shared<Simulation::Backend::Data>(
                std::shared_ptr<PopulationGraph>(tinyGraph->clone()),
            0, nullptr, nullptr, nullptr));

    backend.join();
}

}
