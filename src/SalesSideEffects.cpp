#include "SalesSideEffects.hpp"

#include "CapitalHolder.hpp"

namespace {
    using namespace pyramid_scheme_simulator;

    bool wasRecruitedBy(const std::shared_ptr<Distributor> seller,
            const std::shared_ptr<Distributor> recruitedBy)
    {
        return (*seller->getRecruitedBy()) == (*recruitedBy);
    }
}

namespace pyramid_scheme_simulator {


/**
 * the seller is the bottom of the chain
 * first (index 0) was recruited by the second (index 1) who was recruited by 
 * the third (index 2) and so on
 */
SalesSideEffects::BeneficiaryChain SalesSideEffects::getBeneficiaryChain(
        const std::shared_ptr<Distributor> seller)
{
    //walk the chain of sellers and who recruited them,
    //starting with the person actually making the sale
    BeneficiaryChain beneficiaries;

    std::shared_ptr<Distributor> currentBeneficiary = seller;

    do {
        beneficiaries.emplace_back(currentBeneficiary);
        currentBeneficiary = currentBeneficiary->getRecruitedBy();
    }
    while(currentBeneficiary->getRecruitedBy() != nullptr);

    return beneficiaries;
}

void SalesSideEffects::auditBeneficiaryChain(
        SalesSideEffects::BeneficiaryChain chain)
{
    auto currIt = chain.begin(),
        nextIt = chain.begin(),
        endIt = chain.end();

    while(currIt != endIt)
    {
        //if there's no next then the current distributor shouldn't have a recruitedBy
        if(nextIt == endIt)
        {
            std::shared_ptr<Distributor> seller = *currIt;
            assert(seller->getRecruitedBy().get() == nullptr);
        }
        //otherwise, check they match up
        else
        {
            assert(wasRecruitedBy(*currIt, *nextIt));
        }
    }
}

}

