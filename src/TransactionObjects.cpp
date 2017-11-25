#include "TransactionObjects.hpp"
#include "TransactionRecords.hpp"

#include <memory>
#include <set>
#include <functional>

namespace {
    template <typename T>
    std::function<bool(const std::unique_ptr<T>&, const std::unique_ptr<T>&)>
            mkCmpUniqueables()
    {
        return [](const std::unique_ptr<T>& lhs, 
                const std::unique_ptr<T>& rhs) -> bool {
            return pyramid_scheme_simulator::compareUniqueables(lhs.get(), rhs.get());
        };
    }


    template <typename T>
    std::function<bool(const std::unique_ptr<T>&, const std::unique_ptr<T>&)>
            mkCmpUniques()

    {
        return [](const std::unique_ptr<T>& lhs, 
                const std::unique_ptr<T>& rhs) -> bool {
            return pyramid_scheme_simulator::compareUniques(lhs.get(), rhs.get());
        };
    }
}

namespace pyramid_scheme_simulator {

const ConversionHandler::ComparatorType ConversionHandler::comparator =
    mkCmpUniqueables<ConversionHandler::Conversion>();

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


const RestockHandler::ListComparatorType RestockHandler::listComparator =
    mkCmpUniques<RestockHandler::ElemType>();


const std::set<RestockHandler::ElemType> 
    RestockHandler::toSet(RestockHandler::RecordType&& rec)
{
    std::set<RestockHandler::ElemType> uniques;

    for(std::unique_ptr<Unique>& x : rec.records)
    {
        //copy it
        uniques.emplace<Unique>(std::move(*x));
    }

    return uniques;
}

//const SaleHandler::ComparatorType SaleHandler::comparator =

}
