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
    const std::shared_ptr<const RestockHandler::RestockSet> _restockSet,
    const std::shared_ptr<const SaleHandler::RecordType> _saleRecords)
    : graph(_graph),
    when(_when),
    conversionRecords(_conversionRecords),
    restockSet(_restockSet),
    saleRecords(_saleRecords)
{ }

Simulation::Simulation(Config* c, std::vector<std::unique_ptr<Backend>>&& _backends) 
    : config(std::shared_ptr<Config>(c)),
    conversionHandler(config->randomGen,
            config->simulationOptions->distributionOptions->buyIn.getOption()),
    backends(std::move(_backends))
{
    populationGraph = buildGraph(config);
}

std::unique_ptr<PopulationGraph> Simulation::buildGraph(std::shared_ptr<Config> config)
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

std::shared_ptr<Simulation::Backend::Data> Simulation::cycle()
{
    ConversionHandler::RecordType conversionRecords = applyConversions();

    std::pair<std::shared_ptr<const SaleHandler::RecordType>,
            std::shared_ptr<const RestockHandler::RestockSet>> salesRes = applySales();

    const auto salesRecords = salesRes.first;
    const auto restockSet = salesRes.second;
    const std::shared_ptr<ConversionHandler::RecordType> conversionRecs = 
        std::make_shared<ConversionHandler::RecordType>(std::move(conversionRecords));

    SalesSideEffects::apply(
            config->simulationOptions->distributionOptions->companyPaysCommission,
            config->simulationOptions->distributionOptions->downstreamPercent.getOption(),
            config->simulationOptions->wholesaleProductCost,
            company,
            *salesRes.first);

    const std::shared_ptr<PopulationGraph> graphPtr = 
        std::shared_ptr<PopulationGraph>(populationGraph->clone());

    return std::make_shared<Backend::Data>(graphPtr,
            when(),
            conversionRecs,
            restockSet,
            salesRecords);
}


void Simulation::cycleCallBackends()
{
    const std::shared_ptr<Backend::Data> data = cycle();
    for(const auto& thisBackend : backends)
    {
        thisBackend->exportData(data);
    }
}


std::pair<std::shared_ptr<const SaleHandler::RecordType>,
            std::shared_ptr<const RestockHandler::RestockSet>>
    Simulation::applySales()
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


    return std::make_pair(
            std::make_shared<SaleHandler::RecordType>(std::move(allSalesRecords)), 
            std::make_shared<RestockHandler::RestockSet>(restockSet));
}

ConversionHandler::RecordType Simulation::applyConversions()
{
    //TODO: consider refactoring into another top-level class that can be derived by test classes
    const auto f = [this](std::pair<PopulationGraph::Pop, PopulationGraph::Pop> pops) {
        return this->conversionHandler.operator()(this->when(), pops)
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
        [&recs, this](PopulationGraph::Pop toConvert)
        {
            assert(!toConvert->isDistributor());

            ConversionHandler::Conversion* thisConvertsRecord = 
                Simulation::lookupConversionRecord(recs, toConvert->id);

            if(thisConvertsRecord == nullptr)
            {
                throw ProcessConversionException(
                        STRCAT("Could not find conversion record for ",
                            toConvert->prettyPrint(), 
                            " despite the predicate returning true"));
            }
            else
            {
                //look up the Distributor in the graph
                //TODO: may want to lift this into another method for synchronization reasons
                std::shared_ptr<CapitalHolder> distributor = 
                    this->populationGraph->findVertexPointerByUnique(thisConvertsRecord->convertedBy);

                if(!distributor)
                {
                    throw ProcessConversionException(STRCAT("Could not find the distributor ",
                                "that converted ", toConvert->prettyPrint(), ", distributor id=", 
                                thisConvertsRecord->convertedBy.prettyPrint()));
                }
                else
                {
                    //check after casting it
                    std::shared_ptr<Distributor> dPtr = std::dynamic_pointer_cast<Distributor>(distributor);
                    if(!distributor->isDistributor())
                    {
                        throw ProcessConversionException(STRCAT("distributor->isDistributor() returned false for ",
                                distributor->prettyPrint(), ", the distributor that converted ",
                                toConvert->prettyPrint()));
                    }
                    else if(dPtr == nullptr)
                    {
                        throw ProcessConversionException(STRCAT("Expected a distributor but could not cast ",
                                distributor->prettyPrint(), ", the distributor that converted ",
                                toConvert->prettyPrint()));
                    }
                    else
                    {
                        //throw an exception on error rather than sigsegv
                        return dynamic_cast<Consumer&>(*toConvert)
                            .becomeDistributor(dPtr);
                    }
                }
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
