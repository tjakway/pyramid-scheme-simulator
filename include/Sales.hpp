#pragma once

#include "Types.hpp"
#include "Tick.hpp"
#include "CapitalHolder.hpp"
#include "Unique.hpp"
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


    class SaleIsPossibleResult;

    /**
     * return SalesResult objects to give more information about why the sale
     * didn't go through
     */
    SalesResult sampleSalesChance(rd_ptr, CapitalHolder& seller, CapitalHolder& buyer);
    SaleIsPossibleResult saleIsPossible(CapitalHolder& seller, CapitalHolder& buyer);

    void auditSales();
public:

    SalesResult processPotentialSale(SimulationTick,
            Money price,
            rd_ptr,
            CapitalHolder& seller,
            CapitalHolder& buyer);
     
};

}
