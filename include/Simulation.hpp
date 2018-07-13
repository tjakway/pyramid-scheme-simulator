#pragma once

#include <memory>
#include <vector>
#include <algorithm>

#include "Config.hpp"
#include "PopulationGraph.hpp"
#include "TransactionObjects.hpp"
#include "TransactionRecords.hpp"
#include "Util/NewExceptionType.hpp"
#include "Util/AtomicDeque.hpp"

#include "GraphRecordProcessor.hpp"
namespace pyramid_scheme_simulator {

class Simulation : protected GraphRecordProcessor
{
private:
    std::shared_ptr<Config> config;
    std::unique_ptr<PopulationGraph> populationGraph;
    std::shared_ptr<Company> company;

    ConversionHandler conversionHandler;
    RestockHandler restockHandler;

    SimulationTick now = 0;

    std::unique_ptr<PopulationGraph> buildGraph(std::shared_ptr<Config>);
    
protected:
    virtual SimulationTick when() const override { return now; }

    virtual const PopulationGraph& getPopulationGraph() override { return *populationGraph; }
    virtual const ConversionHandler& getConversionHandler() override { return conversionHandler; }
    virtual const RestockHandler& getRestockHandler() override { return restockHandler; }

    ConversionHandler::RecordType applyConversions();

    std::pair<std::shared_ptr<const SaleHandler::RecordType>,
            std::shared_ptr<const RestockHandler::RestockSet>> applySales();

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
            const std::shared_ptr<const RestockHandler::RestockSet> restockSet;
            const std::shared_ptr<const SaleHandler::RecordType> saleRecords;

            Data(
                const std::shared_ptr<PopulationGraph>,
                const SimulationTick,
                const std::shared_ptr<ConversionHandler::RecordType>,
                const std::shared_ptr<const RestockHandler::RestockSet>,
                const std::shared_ptr<const SaleHandler::RecordType>);

            Data(const Data&);
            virtual ~Data() {}
        };

        virtual void interrupt() noexcept = 0;
        virtual bool interrupted() const noexcept = 0;

        virtual void join() noexcept {}

        virtual void exportData(const std::shared_ptr<Data>) = 0;

        virtual ~Backend() {}
    };

protected:
    std::vector<std::unique_ptr<Backend>> backends;

    std::shared_ptr<Backend::Data> cycle();
    void cycleCallBackends();

public:
    Simulation(Config*, std::vector<std::unique_ptr<Backend>>&&);

    void runCycle();
    void interrupt() const noexcept;
};

} //namespace pyramid_scheme_simulator
