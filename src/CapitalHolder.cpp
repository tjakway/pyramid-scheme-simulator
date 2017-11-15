#include "CapitalHolder.hpp"

namespace pyramid_scheme_simulator {

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

bool Distributor::canPurchase(Money cost, const CapitalHolder& from)
{
    if(*this != from)
    {
        return false;
    }
    else
    {
        //XXX
    }
}

}
