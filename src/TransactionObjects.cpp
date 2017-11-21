#include "TransactionObjects.hpp"
#include "TransactionRecords.hpp"

namespace pyramid_scheme_simulator {

const ConversionHandler::ComparatorType ConversionHandler::comparator =
    [](const std::unique_ptr<ConversionHandler::Conversion>& lhs, 
            const std::unique_ptr<ConversionHandler::Conversion>& rhs) {

        return compareUniqueables(lhs.get(), rhs.get());
    };

RestockHandler::RestockList::RestockList(
        RestockHandler::RestockList::CollectionType&& col)
    :  distributors(col)
{}

const RestockHandler::ComparatorType RestockHandler::comparator = 
    [](const RestockHandler::ElemType& lhs, 
        const RestockHandler::ElemType& rhs)
    {
        return compareUniqueables(lhs.lock().get(), 
                rhs.lock().get());
    };

const std::set<RestockHandler::ElemType, RestockHandler::ComparatorType> 
    RestockHandler::toSet(RestockHandler::RecordType&&)
{
    //TODO
}


//TODO: initialize SaleHandler::comparator

}
