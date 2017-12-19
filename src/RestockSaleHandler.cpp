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

}
