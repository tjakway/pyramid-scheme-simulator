#include "SalesSideEffects.hpp"

#include <unordered_map>
#include <algorithm>
#include <iterator>

#include "Util/Util.hpp"
#include "Util/AssertWithMessage.hpp"
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
const SalesSideEffects::BeneficiaryChain& 
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
    const double downstreamPercent;

    /**
     * this function does the actual work
     */
    static std::unordered_map<Unique, Money> calculatePaymentMapping(
        const double downstreamPercent,
        const Money soldFor,
        const Money wholesalePrice, 
        const BeneficiaryChain chain)
    {
        ASSERT_WITH_MESSAGE(soldFor > wholesalePrice, 
                "Distributors must sell the product for more than its wholesale price.");
        SalesSideEffects::auditBeneficiaryChain(chain);

        //the seller has to make at least enough to cover the cost of buying the product
        const Money guaranteedToSeller = min(1.0, soldFor - wholesalePrice);
        const Money remainder = min(0.0, soldFor - wholesalePrice);

        Money sellerPayment = guaranteedToSeller + ((1 - downstreamPercent) * remainder);

        std::unordered_map<Unique, Money> payments;
        const Unique sellerId = chain.front()->id;

        BeneficiaryChain remainingDistributors;

        auto it = chain.rbegin();
        while((*it)->id != sellerId)
        {
            remainingDistributors.emplace_back(*it);

            it++;
            assert(it != chain.rend());
        }

        Money toDivide = downstreamPercent * remainder;

        //if there's no upstream distributors, award everything to the seller
        if(remainingDistributors.size() <= 0)
        {
            payments.emplace(std::make_pair(sellerId, sellerPayment + toDivide));
        }
        else 
        {
            if(toDivide > 0)
            {
                std::unique_ptr<Unique> lastPaid;
                for(auto reIt = remainingDistributors.begin();
                        reIt != remainingDistributors.end();
                        ++reIt)
                {
                    const Money thisPayment = downstreamPercent * toDivide;
                    toDivide = toDivide - thisPayment;

                    const Unique payee = (*reIt)->id;
                    payments.emplace(payee, thisPayment);

                    lastPaid = make_unique<Unique>(payee);
                }

                //give the last guy anything left
                if(toDivide > 0 && (lastPaid.get() != nullptr))
                {
                    const auto lastPayment = payments.find(*lastPaid);
                    if(lastPayment != payments.end())
                    {
                        const Unique payee((*lastPayment).first);
                        const Money amt = (*lastPayment).second;
                        payments.erase(payee);
                        payments.emplace(std::make_pair(payee, amt + toDivide));
                    }
                }
            }
        }

        return payments;
    }

public:
    ChainedPercentWithGuarantee(
            const double _downstreamPercent,
            const Money soldFor,
            const Money wholesalePrice, 
            const BeneficiaryChain chain)
        : BenefitFormula(soldFor, wholesalePrice, chain),
        downstreamPercent(_downstreamPercent)
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

    ASSERT_WITH_MESSAGE(chain.size() >= 2, 
            "The beneficiary chain has to at least contain the seller and the company");

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


//try and find the company automatically
void SalesSideEffects::auditBeneficiaryChain(BeneficiaryChain chain)
{
    const auto company = std::dynamic_pointer_cast<Company>(chain.back());
    ASSERT_WITH_MESSAGE(company.get() != nullptr, 
            "Could not find the company automatically in auditBeneficiaryChain");

    auditBeneficiaryChain(chain, company);
}

}

