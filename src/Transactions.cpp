#include "Transactions.hpp"
#include "ChanceContributor.hpp"

namespace pyramid_scheme_simulator {

SalesResult Transactions::sell(CapitalHolder& seller, CapitalHolder& buyer)
{

}

SalesResult Transactions::processSalesChance(rd_ptr rd, CapitalHolder& seller, CapitalHolder& buyer)
{
    if(ChanceContributor::sampleFrom(rd, seller.getSalesChanceContribution(),
            buyer.getSalesChanceContribution()))
    {

    }
    else
    {

    }
}



}
