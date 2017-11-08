#include "CapitalHolder.hpp"

namespace pyramid_scheme_simulator {

bool Distributor::canPurchase(const CapitalHolder const& from, unsigned int cost)
{
    if(from != this)
    {
        return false;
    }
}

}
