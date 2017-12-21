#include "SalesSideEffects.hpp"

#include <unordered_map>

#include "Util/Strcat.hpp"
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


/***************************/
/***** Benefit Formula *****/
/***************************/
SalesSideEffects::BenefitFormula::BenefitFormula(const Money _soldFor,
    const Money _wholesalePrice, 
    const BeneficiaryChain _beneficiaryChain)
    : soldFor(_soldFor),
    wholesalePrice(_wholesalePrice),
    beneficiaryChain(_beneficiaryChain)
{ }


void SalesSideEffects::BenefitFormula::unknownBeneficiaryError(
        std::shared_ptr<Distributor> who) const
{
    throw UnknownBeneficiaryException(STRCAT("Could not find ",
                "a payout amount for Distributor with ID ",
                who->id));
}

Money SalesSideEffects::BenefitFormula::getSoldForPrice() const
{
    return soldFor;
}
Money SalesSideEffects::BenefitFormula::getWholesalePrice() const
{
    return wholesalePrice;
}
SalesSideEffects::BeneficiaryChain 
    SalesSideEffects::BenefitFormula::getBeneficiaryChain() const
{
    return beneficiaryChain;
}

Money SalesSideEffects::BenefitFormula::getBenefit(
        std::shared_ptr<Distributor> who) const
{
    return getBenefit(*who);
}

class SalesSideEffects::ChainedPercentWithGuarantee
    : public BenefitFormula
{
protected:
    const std::unordered_map<Unique, Money> paymentMapping;

    /**
     * this function does the actual work
     */
    static std::unordered_map<Unique, Money> calculatePaymentMapping(
        const Money soldFor,
        const Money wholesalePrice, 
        const BeneficiaryChain chain)
    {

    }

public:
    ChainedPercentWithGuarantee(
            const Money soldFor,
            const Money wholesalePrice, 
            const BeneficiaryChain chain)
        : BenefitFormula(soldFor, wholesalePrice, chain)
    {}

    virtual Money getBenefit(std::shared_ptr<Distributor> who) const override
    {
        auto res = paymentMapping.find(who->id);
        if(res == paymentMapping.end())
        {
            unknownBeneficiaryError(who);
        }
        else
        {
            return (*res).second;
        }
    }
};

/***************************/
/***************************/
/***************************/

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
        SalesSideEffects::BeneficiaryChain chain, 
        std::shared_ptr<Distributor> company)
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
            //the last beneficiary should always be the company
            assert((*seller) == (*company));
        }
        //otherwise, check they match up
        else
        {
            assert(wasRecruitedBy(*currIt, *nextIt));
        }
    }
}

}

