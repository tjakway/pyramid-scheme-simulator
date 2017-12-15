#include "TransactionObjects.hpp"

namespace pyramid_scheme_simulator {

RestockHandler::RecordType RestockHandler::operator()(
    SimulationTick when,
    Money price, 
    CapitalHolder& who)
{
    Distributor* dist = dynamic_cast<Distributor*>(&who);

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
