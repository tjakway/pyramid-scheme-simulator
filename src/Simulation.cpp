#include <memory>
#include <iterator>

#include "Tick.hpp"
#include "Config.hpp"
#include "PopulationGraph.hpp"
#include "Simulation.hpp"
#include "SalesSideEffects.hpp"

namespace pyramid_scheme_simulator {


Simulation::Backend::Data::Data(
    const std::shared_ptr<PopulationGraph> _graph,
    const SimulationTick _when,
    const std::shared_ptr<ConversionHandler::RecordType> _conversionRecords,
    const std::shared_ptr<RestockHandler::RecordType> _restockRecords,
    const std::shared_ptr<SaleHandler::RecordType> _saleRecords,
    const PopulationGraph::vertices_size_type _numConversions)
    : graph(_graph),
    when(_when),
    conversionRecords(_conversionRecords),
    restockRecords(_restockRecords),
    saleRecords(_saleRecords),
    numConversions(_numConversions)
{ }

Simulation::Simulation(Config* c, std::vector<std::unique_ptr<Backend>>&& _backends) 
    : config(std::shared_ptr<Config>(c)),
    conversionHandler(config->randomGen,
            config->simulationOptions->distributionOptions->buyIn.getOption()),
    backends(std::move(_backends))
{
    populationGraph = buildGraph(config);
}

std::unique_ptr<PopulationGraph> buildGraph(std::shared_ptr<Config> config)
{
    return std::unique_ptr<PopulationGraph>(new PopulationGraph(*config));
}


void Simulation::interrupt() const noexcept
{
    //interrupt each backend to give them a chance to shut down
    for(const auto& backend : backends)
    {
        backend->interrupt();
    }

    //wait for them to finish shutting down
    for(const auto& backend: backends)
    {
        backend->join();
    }
}

void Simulation::tick()
{
    const ConversionHandler::RecordType conversionRecords = applyConversions();
    const SaleHandler::RecordType saleRecords = applySales();


    SalesSideEffects::apply(
            config->simulationOptions->distributionOptions->companyPaysCommission,
            config->simulationOptions->distributionOptions->downstreamPercent.getOption(),
            config->simulationOptions->wholesaleProductCost,
            company,
            saleRecords);

}

SaleHandler::RecordType Simulation::applySales()
{
    std::vector<RestockHandler::RecordType> restockRecords = 
        populationGraph->forEachVertex<RestockHandler::RecordType>
        ([this](PopulationGraph::Pop p) -> RestockHandler::RecordType {
            return this->restockHandler(
                    this->when(),
                    this->config->simulationOptions->wholesaleProductCost,
                    *p);
            });

    const RestockHandler::RestockSet restockSet = RestockHandler::toSet(
            emptyListTransactionRecord<RestockHandler::ElementType>().leftFold(
                std::list<RestockHandler::RecordType>(
                    std::make_move_iterator(restockRecords.begin()),
                    std::make_move_iterator(restockRecords.end())),
                RestockHandler::reduce));

    SaleHandler saleHandler(restockSet);

    std::vector<SaleHandler::RecordType> saleRecordsVec = 
        populationGraph->forEachEdge<SaleHandler::RecordType>
        ([&saleHandler, this](std::pair<PopulationGraph::Pop, PopulationGraph::Pop> edge) 
            -> SaleHandler::RecordType {
            return saleHandler(
                    this->when(),
                    this->config->simulationOptions->standardProductCost,
                    this->config->randomGen,
                    *edge.first,
                    *edge.second);
            });

    SaleHandler::RecordType saleRecords =
        emptyListTransactionRecord<SaleHandler::ElementType>().leftFold(
                        std::list<SaleHandler::RecordType>(
                            std::make_move_iterator(saleRecordsVec.begin()),
                            std::make_move_iterator(saleRecordsVec.end())),
                        SaleHandler::reduce);

    RestockSaleHandler restockSaleHandler(
            config->simulationOptions->wholesaleProductCost,
            company,
            restockSet);

    SaleHandler::RecordType allSalesRecords = 
        restockSaleHandler(when(), *populationGraph, this->config->randomGen)
            .leftFold(std::move(saleRecords), SaleHandler::reduce);


    //TODO: apply sales side effects
    return allSalesRecords;
}

ConversionHandler::RecordType Simulation::applyConversions()
{
    const std::function<ConversionHandler::RecordType(std::pair<PopulationGraph::Pop,
            PopulationGraph::Pop>)> f =
        [this](std::pair<PopulationGraph::Pop,
            PopulationGraph::Pop> pops)
            -> ConversionHandler::RecordType
        {
            CapitalHolder& a = *(pops.first);
            CapitalHolder& b = *(pops.second);
            return this->conversionHandler.operator()(this->when(), 
                    a,
                    b);
                    
        };

    std::vector<ConversionHandler::RecordType> vecConversions = 
        populationGraph->forEachEdge(f);

    auto conversionRecs = 
        emptyListTransactionRecord<ConversionHandler::ElementType>().leftFold(
                std::list<ConversionHandler::RecordType>(
                    std::make_move_iterator(vecConversions.begin()),
                    std::make_move_iterator(vecConversions.end())),
                conversionHandler.reduce);

    const auto numConversions = processConversions(conversionRecs);
    assert(numConversions == conversionRecs.records.size());
    return conversionRecs;
}

ConversionHandler::Conversion* Simulation::lookupConversionRecord(
        ConversionHandler::RecordType& recs,
        const Unique possibleConvertId)
{
    using RecIt = ConversionHandler::RecordType::ContainerType::iterator;
    RecIt res = std::find_if(recs.records.begin(), recs.records.end(),
            [possibleConvertId](const std::unique_ptr<ConversionHandler::ElementType>& x){
                return x->id == possibleConvertId;
            });

    if(res == recs.records.end())
    {
        return nullptr;
    }
    else
    {
        return (*res).get();
    }
}


PopulationGraph::vertices_size_type 
    Simulation::processConversions(ConversionHandler::RecordType& recs)
{
    NEW_EXCEPTION_TYPE(ProcessConversionException);

    PopulationGraph::MutateVertexFunction mutate = 
        [&recs](PopulationGraph::Pop toConvert)
        {
            assert(!toConvert->isDistributor());

            ConversionHandler::Conversion* thisConvertsRecord = 
                Simulation::lookupConversionRecord(recs, toConvert->id);

            if(thisConvertsRecord == nullptr)
            {
                throw ProcessConversionException(
                        STRCAT("Could not find conversion record for ",
                            toConvert->id, " despite the predicate returning true"));
            }
            else
            {
                //throw an exception on error rather than sigsegv
                return dynamic_cast<Consumer&>(*toConvert)
                    .becomeDistributor(thisConvertsRecord->convertedBy);
            }
        };

    PopulationGraph::VertexPredicate predicate = 
        [&recs](const CapitalHolder& x)
        {
            //filter for any CapitalHolder's we have conversion records for
            return lookupConversionRecord(recs, x.id) != nullptr;
        };

    return populationGraph->mutateVerticesWithPredicate(mutate, predicate);
}


}
