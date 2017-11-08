#pragma once

#include <functional>
#include <random>

#include "RDType.hpp"
#include "CapitalHolder.hpp"


namespace pyramid_scheme_simulator {

/**
 * return a sales function that is totally random
 */
Distributor::SalesFunction mkRandomSalesFunction(const unsigned int productCost,
        rd_ptr randomGen) {
    return [=](const CapitalHolder const& potentialBuyer) -> double {
        if(potentialBuyer.canPurchase(productCost)) {
            return std::uniform_real_distribution()(randomGen);
        }
        //can't buy it without money
        else {
            return 0;
        }
    };
}

}
