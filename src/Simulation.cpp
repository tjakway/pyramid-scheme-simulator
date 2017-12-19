#include <memory>
#include <iterator>

#include "Tick.hpp"
#include "Config.hpp"
#include "PopulationGraph.hpp"
#include "Simulation.hpp"

namespace pyramid_scheme_simulator {

Simulation::Simulation(Config* c, std::vector<std::unique_ptr<Backend>>&& _backends) 
    : config(std::shared_ptr<Config>(c)),
    conversionHandler(config->randomGen,
            config->simulationOptions->distributionOptions->buyIn),
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
    const PopulationGraph::vertices_size_type numConversions = applyConversions();




}

PopulationGraph::vertices_size_type Simulation::applySales()
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
                std::move(restockRecords),
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

    const SaleHandler::RecordType saleRecords =
        emptyListTransactionRecord<SaleHandler::ElementType>().leftFold(
                        std::move(saleRecordsVec),
                        SaleHandler::reduce);

    RestockSaleHandler restockSaleHandler(
            config->simulationOptions->wholesaleProductCost,
            company,
            restockSet);

    const SaleHandler::RecordType allSalesRecords = 
        restockSaleHandler(when(), *populationGraph, this->config->randomGen)
            .leftFold(std::move(saleRecords), SaleHandler::reduce);

}

PopulationGraph::vertices_size_type Simulation::applyConversions()
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

    //TODO: specialize foldLeft for vectors to avoid the vector -> list conversion
    //or overload foldLeft to take a begin and end iterator that it can construct a
    //collection out of
    
    auto conversionRecs = 
        emptyListTransactionRecord<ConversionHandler::ElementType>().leftFold(
                std::list<ConversionHandler::RecordType>(
                    std::make_move_iterator(vecConversions.begin()),
                    std::make_move_iterator(vecConversions.end())),
                conversionHandler.reduce);

    return processConversions(conversionRecs);
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
