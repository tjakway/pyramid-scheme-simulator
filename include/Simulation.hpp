#pragma once

#include <memory>
#include <vector>
#include <algorithm>

#include "Config.hpp"
#include "PopulationGraph.hpp"
#include "TransactionObjects.hpp"
#include "TransactionRecords.hpp"
#include "Util/NewExceptionType.hpp"

namespace pyramid_scheme_simulator {

class Simulation
{
private:
    std::shared_ptr<Config> config;
    std::unique_ptr<PopulationGraph> populationGraph;

    ConversionHandler conversionHandler;

    SimulationTick now = 0;
    SimulationTick when() const { return now; }

    std::unique_ptr<PopulationGraph> buildGraph(std::shared_ptr<Config>);
    void tick();
    
protected:
    static ConversionHandler::Conversion* lookupConversionRecord(
            ConversionHandler::RecordType&,
            Unique);

    PopulationGraph::vertices_size_type 
        processConversions(ConversionHandler::RecordType&);

public:
    class Backend
    {
    public:
        //base class for any backend-related exceptions
        NEW_EXCEPTION_TYPE(BackendException);

        class Data
        {
        public:
            const std::shared_ptr<PopulationGraph> graph;
            const SimulationTick when;

            const std::shared_ptr<ConversionHandler::RecordType> conversionRecords;
            const std::shared_ptr<RestockHandler::RecordType> restockRecords;
            const std::shared_ptr<SaleHandler::RecordType> saleRecords;
        };

        virtual void interrupt() noexcept = 0;
        virtual bool interrupted() const noexcept = 0;

        virtual void join() noexcept {}

        virtual void exportData(const std::shared_ptr<Data>) = 0;
    };

private:
    std::vector<std::unique_ptr<Backend>> backends;

public:
    Simulation(Config*, std::vector<std::unique_ptr<Backend>>&&);

    void interrupt() const noexcept;
};

} //namespace pyramid_scheme_simulator
