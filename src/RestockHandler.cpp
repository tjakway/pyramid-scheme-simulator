#include "TransactionObjects.hpp"

namespace pyramid_scheme_simulator {

RestockHandler::RecordType RestockHandler::operator()(
    const SimulationTick /*when*/,
    const Money /*price*/, 
    const CapitalHolder& who)
{
    const Distributor* dist = dynamic_cast<const Distributor*>(&who);

    auto empty = emptyListTransactionRecord<RestockHandler::ElementType>();
    if(dist == nullptr)
    {
        return empty;
    }
    else if(dist->needsRestock())
    {
        //copy the unique
        return singleElementListTransactionRecord<
            RestockHandler::ElementType>(make_unique<Unique>(dist->id));
    }
    else
    {
        return empty;
    }
}

}
