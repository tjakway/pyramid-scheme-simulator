#include "TransactionObjects.hpp"
#include "TransactionRecords.hpp"
#include "Util/Unique.hpp"

#include <memory>
#include <set>
#include <functional>
#include <utility>

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
    std::unique_ptr<ConversionHandler::RecordType> conversionRecord;
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
    ConversionPredicateResult(std::unique_ptr<ConversionHandler::RecordType>&& rec)
        : ConversionPredicateResult(SUCCESS, std::move(rec), std::string())
    {}

    ConversionPredicateResult(const Result _status, std::string _msg)
        : ConversionPredicateResult(_status, nullptr, _msg)
    {}


    /**
     * move constructor
     */
    ConversionPredicateResult(ConversionPredicateResult&& other)
        : ConversionPredicateResult(other.status, 
                std::move(other.conversionRecord),
                other.msg) {}

    /**
     * master constructor
     */
    ConversionPredicateResult(const Result _status, 
            std::unique_ptr<ConversionHandler::RecordType>&& rec,
            std::string _msg)
        : conversionRecord(std::move(rec)), msg(_msg), status(_status)
    {}


    NEW_EXCEPTION_TYPE(ConversionPredicateResultException);

public:

    const Result status;

    bool success() const
    {
        return status == SUCCESS;
    }

    operator bool() const
    {
        return success();
    }

    static ConversionPredicateResult castFailed(
            const Consumer* consumer, const Unique& consumerId,
            const Distributor* distributor, const Unique& distributorId)
    {
        std::string msg;

        if(consumer == nullptr && distributor == nullptr)
        {
            msg = STRCAT("Neither entity (id = ", consumerId.prettyPrint(), 
                ", id = ", distributorId.prettyPrint(), "),"
                "could be cast to Consumer or Distributor");
        }
        else if(consumer == nullptr && distributor != nullptr)
        {
            msg = STRCAT("A distributor was found (", distributor->prettyPrint(), 
                    ") but the other entity (id=", consumerId.prettyPrint(), 
                    ") could not be cast to a consumer");
        }
        else if(consumer != nullptr && distributor == nullptr)
        {
            msg = STRCAT("A consumer was found (", consumer->prettyPrint(), "),",
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
                STRCAT("Consumer id=", consumer.prettyPrintId(), " canBecomeDistributor returned false"));
    }

    static ConversionPredicateResult success(ConversionHandler::RecordType&& rec)
    {
        return ConversionPredicateResult(std::unique_ptr<ConversionHandler::RecordType>(rec));
    }
};



ConversionPredicateResult ConversionHandler::testConversion(
        rd_ptr rd,
        const SimulationTick when,
        const Consumer& consumer,
        const Distributor& distributor,
        const Money buyIn)
{
    if(consumer.canBecomeDistributor(buyIn))
    {
        const auto combinedChanceContributor = consumer.getDistributorConversionChanceContribution() +
            distributor.getDistributorConversionChanceContribution();
            
        if(combinedChanceContributor->sampleFrom(rd))
        {
            return ConversionPredicateResult::success(
                    Conversion::RecordType(when, consumer.id, distributor.id));
        }
        else
        {
            return ConversionPredicateResult::procFailed(*combinedChanceContributor);
        }
    }
    else
    {
        return ConversionPredicateResult::cantBeDistributor(consumer);
    }
}

ConversionPredicateResult ConversionHandler::predF(const CapitalHolder& lhs, const CapitalHolder& rhs)
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
        return ConversionPredicateResult::castFailed(consumer, lhs.id, 
                distributor, rhs.id);
    }
    else
    {
        return testConversion(rd, when, *consumer, *distributor, buyIn);
    }
}


ConversionHandler::ConversionHandler(rd_ptr _rd, const Money _buyIn)
    : rd(_rd), 
    buyIn(_buyIn),
    predicate(
        [this](const CapitalHolder& lhs, const CapitalHolder& rhs) { 
            return predF(lhs, rhs);
        })
{}




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
