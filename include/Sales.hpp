#pragma once

#include "Types.hpp"
#include "Tick.hpp"
#include "CapitalHolder.hpp"
#include "Util/Unique.hpp"
#include "SalesRecords.hpp"

#include <memory>
#include <set>
#include <string>
#include <unordered_set>

namespace pyramid_scheme_simulator {


/**
 * global sales records
 */
class Transactions
{
    std::set<Sale> sales;
    /**
     * Consumers who were turned into Distributors
     */
    std::set<Conversion> conversions;

    /**
     * unique Distributor instance that has unlimited inventory and from which
     * all other Distributors are forced to buy
     */
    const std::unique_ptr<Distributor> companyDistributor;

    class SaleIsPossibleResult;

    /**
     * return SalesResult objects to give more information about why the sale
     * didn't go through
     */
    SalesResult sampleSalesChance(rd_ptr, CapitalHolder& seller, CapitalHolder& buyer);
    SaleIsPossibleResult saleIsPossible(CapitalHolder& seller, CapitalHolder& buyer);

    void auditRecords();
public:

    /**
     * parameterized with price to allow for cheaper wholesale purchases from the company
     */
    void processPotentialRestocking(SimulationTick, Money, CapitalHolder&);
    void processPotentialConversion(CapitalHolder&);

    


    SalesResult processPotentialSale(SimulationTick,
            Money price,
            rd_ptr,
            CapitalHolder& seller,
            CapitalHolder& buyer);
     
};

}
