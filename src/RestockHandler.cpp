#include "TransactionObjects.hpp"

namespace pyramid_scheme_simulator {

RestockHandler::RecordType RestockHandler::operator()(
    SimulationTick when,
    Money price, 
    CapitalHolder& who)
{
    Distributor* dist = dynamic_cast<Distributor*>(&who);

    const auto empty = emptyListTransactionRecord<RestockHandler::RecordType>();
    if(dist == nullptr)
    {
        return empty;
    }
    else if(dist->needsRestock())
    {
        return singleElementListTransactionRecord<
            RestockHandler::RecordType>(dist->id);
    }
    else
    {
        return empty;
    }
}

}
