#include "TransactionObjects.hpp"
#include "TransactionRecords.hpp"

#include <memory>
#include <set>

namespace pyramid_scheme_simulator {

const ConversionHandler::ComparatorType ConversionHandler::comparator =
    [](const std::unique_ptr<ConversionHandler::Conversion>& lhs, 
            const std::unique_ptr<ConversionHandler::Conversion>& rhs) -> bool {

        return compareUniqueables(lhs.get(), rhs.get());
    };

const std::function<ConversionHandler::RecordType(
        ConversionHandler::RecordType&&, 
        ConversionHandler::RecordType&&)> 
    ConversionHandler::reduce =
[](ConversionHandler::RecordType&& lhs, ConversionHandler::RecordType&& rhs){ 
    return mergeListTransactionRecords<ConversionHandler::ElemType>(
            std::move(lhs), std::move(rhs), ConversionHandler::comparator);
};


const std::function<SaleHandler::RecordType(
        SaleHandler::RecordType&&, 
        SaleHandler::RecordType&&)> SaleHandler::reduce =
[](SaleHandler::RecordType&& lhs, SaleHandler::RecordType&& rhs){ 
    return mergeListTransactionRecords<SaleHandler::ElemType>(
            std::move(lhs), std::move(rhs), SaleHandler::comparator);
};

const std::function<RestockHandler::RecordType(
        RestockHandler::RecordType&&, 
        RestockHandler::RecordType&&)> RestockHandler::reduce =
[](RestockHandler::RecordType&& lhs, RestockHandler::RecordType&& rhs){ 
    return mergeListTransactionRecords<RestockHandler::ElemType>(
            std::move(lhs), std::move(rhs), RestockHandler::listComparator);
};

const RestockHandler::SetComparatorType RestockHandler::setComparator = 
    [](const RestockHandler::ElemType& lhs, 
        const RestockHandler::ElemType& rhs)
    {
        return compareUniqueables(lhs.lock().get(), 
                rhs.lock().get());
    };


const RestockHandler::ListComparatorType RestockHandler::listComparator =
    [](const std::unique_ptr<RestockHandler::ElemType>& lhs, 
            const std::unique_ptr<RestockHandler::ElemType>& rhs) -> bool {
        //yikes
        return compareUniqueables(lhs.get()->lock().get(), 
                rhs.get()->lock().get());
    };


const std::set<RestockHandler::ElemType, RestockHandler::ComparatorType> 
    RestockHandler::toSet(RestockHandler::RecordType&& rec)
{
    return std::set<RestockHandler::ElemType, 
           RestockHandler::ComparatorType>
               (std::make_move_iterator(rec.records.begin()),
                    std::make_move_iterator(rec.records.end()),
                RestockHandler::comparator);
}


//TODO: initialize SaleHandler::comparator

}
