#pragma once

#include "Types.hpp"
#include "Tick.hpp"
#include "TransactionRecords.hpp"
#include "CapitalHolder.hpp"
#include "Util/Unique.hpp"

#include <memory>
#include <set>
#include <string>
#include <unordered_set>

namespace pyramid_scheme_simulator {


/**
 * global records
 */
class GraphRecords
{
    /**
     * unique Distributor instance that has unlimited inventory and from which
     * all other Distributors are forced to buy
     */
    const std::unique_ptr<Distributor> companyDistributor;


//    void auditRecords();
public:

    /**
     * parameterized with price to allow for cheaper wholesale purchases from the company
     */
//    void processPotentialRestocking(SimulationTick, Money, CapitalHolder&);
//    void processPotentialConversion(CapitalHolder&);

    
};

}
