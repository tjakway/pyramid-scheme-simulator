#include "CapitalHolder.hpp"

namespace pyramid_scheme_simulator {

double CapitalHolder::getMoneyToProductCostRatio()
{
    return money / productCost;
}

/**
 * compare unique IDs
 */
bool CapitalHolder::operator==(const CapitalHolder& other)
{
    return this->id == other.id;
}

bool CapitalHolder::operator!=(const CapitalHolder& other)
{
    return this->id != other.id;
}

bool Distributor::canPurchase(const CapitalHolder& from, Money cost)
{
    if(*this != from)
    {
        return false;
    }
}

}
