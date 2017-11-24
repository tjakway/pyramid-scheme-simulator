#pragma once

#include "TransactionObjects.hpp"

namespace pyramid_scheme_simulator {
class MockSaleHandler : public SaleHandler
{
public:
    MockSaleHandler()
        //construct with an empty restock list
        : SaleHandler(std::set<Unique>())
    {}
};

}
