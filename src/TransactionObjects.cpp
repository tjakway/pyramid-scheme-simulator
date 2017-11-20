#include "TransactionObjects.hpp"

namespace pyramid_scheme_simulator {


bool ConversionTransaction::ConversionRecords::cmp(
        std::unique_ptr<Conversion>& lhs, 
        std::unique_ptr<Conversion>& rhs)
{
    return compareUniqueables(lhs, rhs);
}


}
