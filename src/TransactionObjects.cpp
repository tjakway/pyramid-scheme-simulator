#include "TransactionObjects.hpp"
#include "TransactionRecords.hpp"

namespace pyramid_scheme_simulator {

ConversionHandler::ComparatorType comparator =
    [](const std::unique_ptr<ConversionHandler::Conversion>& lhs, 
            const std::unique_ptr<ConversionHandler::Conversion>& rhs) {

        return compareUniqueables(lhs.get(), rhs.get());
    };

ConversionHandler::RecordType 
    ConversionHandler::reduce(ConversionHandler::RecordType&& lhs, 
            ConversionHandler::RecordType&& rhs)
    {
        return mergeListTransactionRecords(std::move(lhs), 
                std::move(rhs), ConversionHandler::comparator);
    }
}
