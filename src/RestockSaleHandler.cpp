#include "TransactionObjects.hpp"

namespace pyramid_scheme_simulator {

SaleHandler::RecordType RestockSaleHandler::processRestocksWithPops(
        const SimulationTick when,
        std::set<PopulationGraph::Pop> restockPops, 
        rd_ptr rd)
{
    //make a sale handler without a restock list
    //we'll use this to compute the actual sales records
    SaleHandler saleHandler(RestockHandler::RestockSet{});

    std::list<SaleHandler::RecordType> restockSaleRecords;
    for(const auto& thisPop : restockPops)
    {
        restockSaleRecords.emplace_back(saleHandler(when, 
            wholesalePrice, 
            rd, 
            *this->company,
            *thisPop));
    }

    return emptyListTransactionRecord<SaleHandler::ElementType>().leftFold(
            std::move(restockSaleRecords),
            saleHandler.reduce);
}


std::set<PopulationGraph::Pop> RestockSaleHandler::lookupRestockPops(
        const PopulationGraph& graph,
        const RestockHandler::RestockSet& restockSet)
{
    std::set<PopulationGraph::Pop> restockPops;

    const auto vertices = graph.vertices();
    std::set<PopulationGraph::Pop> allPops(vertices.begin(), vertices.end());

    for(const Unique& thisId : restockSet)
    {
        const auto foundRes = std::find_if(allPops.begin(), allPops.end(),
                [&thisId](PopulationGraph::Pop x)
                {
                    return x->id == thisId;
                });

        if(foundRes == allPops.end())
        {
            throw RestockPopNotFoundException(
                    STRCAT("Unique ", thisId, " has no corresponding ",
                        "pop in passed PopulationGraph"));
        }
        else
        {
            restockPops.emplace(*foundRes);
        }
    }

    return restockPops;
}


SaleHandler::RecordType RestockSaleHandler::processRestocks(
        const SimulationTick when, 
        const PopulationGraph& graph,
        rd_ptr rd)
{
    return processRestocksWithPops(
            when, 
            lookupRestockPops(graph, restockSet),
            rd);
}

//synonym for processRestocks
SaleHandler::RecordType RestockSaleHandler::operator()(
        const SimulationTick when, 
        const PopulationGraph& graph,
        rd_ptr rd)
{
    return processRestocks(when, graph, rd);
}

RestockSaleHandler::RestockSaleHandler(
    const Money _wholesalePrice, 
    std::shared_ptr<Company> _company,
    const RestockHandler::RestockSet& _restockSet)
    : company(_company),
    wholesalePrice(_wholesalePrice),
    restockSet(_restockSet)
{ }

}
