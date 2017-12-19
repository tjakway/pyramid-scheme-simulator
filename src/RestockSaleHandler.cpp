#include "TransactionObjects.hpp"

SaleHandler::RecordType RestockSaleHandler::processRestocksWithPops(
        const SimulationTick when,
        const Money wholesalePrice, 
        std::set<PopulationGraph::Pop> restockPops, 
        rd_ptr rd)
{
    //make a sale handler without a restock list
    //we'll use this to compute the actual sales records
    SaleHandler saleHandler(RestockHandler::RestockSet{});

    std::vector<SaleHandler::RecordType> restockSaleRecords = 
        Util::mapCollection(restockPops, 
        [when, &saleHandler, wholesalePrice, rd, this](PopulationGraph::Pop thisPop){
            return saleHandler(when, 
                    wholesalePrice, 
                    rd, 
                    *this->company,
                    *thisPop);
        });

    emptyListTransactionRecord<ConversionHandler::ElementType>().leftFold(
            std::list<ConversionHandler::RecordType>(
                std::make_move_iterator(vecConversions.begin()),
                std::make_move_iterator(vecConversions.end())),
            conversionHandler.reduce);
}
