#include "TransactionObjects.hpp"

namespace pyramid_scheme_simulator {

SaleHandler::RecordType RestockSaleHandler::processRestocksWithPops(
        const SimulationTick when,
        const Money wholesalePrice, 
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

}
