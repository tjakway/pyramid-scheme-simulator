#include "TransactionObjects.hpp"
#include "TransactionRecords.hpp"
#include "Util/Unique.hpp"

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


    bool cmpUniquePtrsToUnique(const std::unique_ptr<pyramid_scheme_simulator::Unique>& lhs, 
            const std::unique_ptr<pyramid_scheme_simulator::Unique>& rhs)
    {
        return compareUniques(lhs.get(), rhs.get());
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
    return mergeListTransactionRecords<ConversionHandler::ElementType>(
            std::move(lhs), std::move(rhs), ConversionHandler::comparator);
};


bool ConversionHandler::testConversion(rd_ptr rd,
        const Consumer& consumer,
        const Distributor& distributor,
        const Money buyIn)
{
    return consumer.canBecomeDistributor(buyIn) && 
        (consumer.getDistributorConversionChanceContribution() +
         distributor.getDistributorConversionChanceContribution())->sampleFrom(rd);
}

bool ConversionHandler::predF(const CapitalHolder& lhs, const CapitalHolder& rhs)
{
    const Consumer*    consumer = nullptr;
    const Distributor* distributor = nullptr;

    const auto tryCastConsumer = [&consumer](const CapitalHolder* which)
    {
        //skip if we're already found the consumer
        if(consumer == nullptr)
        {
            const Consumer* castConsumer = dynamic_cast<const Consumer*>(which);
            if(castConsumer != nullptr 
                    //a distributor can also be a consumer
                    && !castConsumer->isDistributor()) {
                consumer = castConsumer;
            }
        }
    };

    //unfortunately no template lambdas yet
    const auto tryCastDistributor = [&distributor](const CapitalHolder* which)
    {
        if(distributor == nullptr)
        {
            const Distributor* castDistributor = dynamic_cast<const Distributor*>(which);
            if(castDistributor != nullptr) {
                distributor = castDistributor;
            }
        }
    };

    //try all combinations of consumers and distributors
    tryCastConsumer(&lhs);
    tryCastConsumer(&rhs);
    tryCastDistributor(&lhs);
    tryCastDistributor(&rhs);


    if(consumer == nullptr || distributor == nullptr)
    {
        return false;
    }
    else
    {
        return testConversion(rd, *consumer, *distributor, buyIn);
    }
}

const std::function<SaleHandler::RecordType(
        SaleHandler::RecordType&&, 
        SaleHandler::RecordType&&)> SaleHandler::reduce =
[](SaleHandler::RecordType&& lhs, SaleHandler::RecordType&& rhs){ 
    return mergeListTransactionRecords<SaleHandler::ElementType>(
            std::move(lhs), std::move(rhs), SaleHandler::comparator);
};

const std::function<RestockHandler::RecordType(
        RestockHandler::RecordType&&, 
        RestockHandler::RecordType&&)> RestockHandler::reduce =
[](RestockHandler::RecordType&& lhs, RestockHandler::RecordType&& rhs){ 
    return mergeListTransactionRecords<RestockHandler::ElementType>(
            std::move(lhs), std::move(rhs), RestockHandler::listComparator);
};


const RestockHandler::ListComparatorType RestockHandler::listComparator =
    cmpUniquePtrsToUnique;


const std::set<RestockHandler::ElementType> 
    RestockHandler::toSet(RestockHandler::RecordType&& rec)
{
    std::set<RestockHandler::ElementType> uniques;

    for(std::unique_ptr<Unique>& x : rec.records)
    {
        //copy it
        uniques.emplace<Unique>(std::move(*x));
    }

    return uniques;
}

const SaleHandler::ComparatorType SaleHandler::comparator =
    Util::deepCompareUniquePtrs<SaleHandler::ElementType>;

}
