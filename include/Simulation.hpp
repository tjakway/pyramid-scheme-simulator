#pragma once

#include <memory>
#include <vector>

#include "Config.hpp"
#include "PopulationGraph.hpp"
#include "Util/NewExceptionType.hpp"

namespace pyramid_scheme_simulator {

class Simulation
{
private:
    std::shared_ptr<Config> config;
    std::unique_ptr<PopulationGraph> graph;

    std::unique_ptr<PopulationGraph> buildGraph(std::shared_ptr<Config>);
    void tick();

public:
    class Backend
    {
    public:
        //base class for any backend-related exceptions
        NEW_EXCEPTION_TYPE(BackendException);

        virtual void interrupt() = 0;
        virtual bool interrupted() const = 0;

        virtual void exportGraph(std::shared_ptr<PopulationGraph>, const SimulationTick) = 0;
    };

private:
    std::vector<std::unique_ptr<Backend>> backends;

public:
    Simulation(Config*, std::vector<std::unique_ptr<Backend>>&&);
};

} //namespace pyramid_scheme_simulator
