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


ConversionHandler::RecordType ConversionHandler::operator()(SimulationTick,
    CapitalHolder&, 
    CapitalHolder&)
{
    //TODO: implement
    return emptyListTransactionRecord<ConversionHandler::ElementType>();
}

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

class ConversionHandler::ConversionPredicateResult
{
    const std::shared_ptr<ConversionHandler::RecordType> conversionRecord;
    const std::string msg;

public:
    enum Result
    {
        SUCCESS,
        CAST_FAILED,
        PROC_FAILED,
        CANT_BE_DISTRIBUTOR
    };

protected:
    /**
     * success constructor (no need to pass a result code or message)
     */
    ConversionPredicateResult(std::shared_ptr<ConversionHandler::RecordType> rec)
        : ConversionPredicateResult(SUCCESS, rec, std::string())
    {}

    ConversionPredicateResult(const Result _status, std::string _msg)
        : ConversionPredicateResult(_status, nullptr, _msg)
    {}

    /**
     * master constructor
     */
    ConversionPredicateResult(const Result _status, 
            std::shared_ptr<ConversionHandler::RecordType> rec,
            std::string _msg)
        : status(_status), conversionRecord(rec), msg(_msg)
    {}

    NEW_EXCEPTION_TYPE(ConversionPredicateResultException);

public:

    const Result status;

    static ConversionPredicateResult castFailed(const Consumer* consumer, const Distributor* distributor)
    {
        std::string msg;

        if(consumer == nullptr && distributor == nullptr)
        {
            msg = "Neither entity could be cast to Consumer or Distributor";
        }
        else if(consumer == nullptr && distributor != nullptr)
        {
            msg = STRCAT("A distributor was found (id=", distributor->prettyPrintId(), 
                    ") but the other entity could not be cast to a consumer");
        }
        else if(consumer != nullptr && distributor == nullptr)
        {
            msg = STRCAT("A consumer was found (id=", consumer->prettyPrintId(), "),",
                    " but the other entity could not be cast to a distributor");
        }
        else
        {
            //an error function shouldn't be called if there isn't an error
            throw ConversionPredicateResultException(STRCAT(
                "Method called but expected either consumer or distributor to be nullptr",
                ": consumer id=", consumer->prettyPrintId(), 
                ", distributor id=", distributor->prettyPrintId()));
        }

        return ConversionPredicateResult(CAST_FAILED, msg);
    }

    static ConversionPredicateResult procFailed(const ChanceContributor& cc)
    {
        return ConversionPredicateResult(PROC_FAILED, 
                STRCAT("ChanceContributor ", cc.prettyPrint(), " failed to proc"));
    }

    static ConversionPredicateResult cantBeDistributor(const Consumer& consumer)
    {
        return ConversionPredicateResult(CANT_BE_DISTRIBUTOR, 
                STRCAT("Consumer id=", consumer.prettPrintId(), " canBecomeDistributor returned false"));
    }
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
