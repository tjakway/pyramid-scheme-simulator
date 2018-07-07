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

std::shared_ptr<Distributor> SalesSideEffects::getSeller(
        const BeneficiaryChain& chain)
{
    return chain.front();
}

//BenefitFormula plus company, buyer, and downstream percent (% commission) fields
class SalesSideEffects::EffectTransferable : public BenefitFormula
{
protected:
    const double downstreamPercent;
    std::shared_ptr<Distributor> company;
    std::shared_ptr<Distributor> seller;
    std::shared_ptr<Consumer> buyer;

public:
    EffectTransferable(
            const double _downstreamPercent,
            std::shared_ptr<Distributor> _company,
            std::shared_ptr<Consumer> _buyer,
            const Money soldFor,
            const Money wholesalePrice, 
            const BeneficiaryChain chain)
        : BenefitFormula(soldFor, wholesalePrice, chain),
        downstreamPercent(_downstreamPercent),
        company(_company),
        seller(SalesSideEffects::getSeller(getBeneficiaryChain())),
        buyer(_buyer)
    {}

    virtual ~EffectTransferable() {}

public:
    virtual void effectTransfers()
    {
        buyer->deductMoney(getSoldForPrice());

        std::for_each(getBeneficiaryChain().begin(), getBeneficiaryChain().end(),
                [this](std::shared_ptr<Distributor> x){
                    this->effectTransfer(x);
                });
    }

protected:
    virtual void  effectTransfer(std::shared_ptr<Distributor> to) = 0;
};

class SalesSideEffects::ChainedPercentWithGuarantee
    : public EffectTransferable 
{
protected:
    const std::unordered_map<Unique, Money> paymentMapping;

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
            std::shared_ptr<Distributor> _company,
            std::shared_ptr<Consumer> _buyer,
            const Money _soldFor,
            const Money _wholesalePrice, 
            const BeneficiaryChain _chain)
        : EffectTransferable(
                _downstreamPercent,
                _company,
                _buyer, 
                _soldFor,
                _wholesalePrice,
                _chain),
        paymentMapping(calculatePaymentMapping(
                    _downstreamPercent,
                    _soldFor,
                    _wholesalePrice,
                    _chain))
    {}

    virtual ~ChainedPercentWithGuarantee() {}

protected:
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

    //trivial implementation, just look up the amount of money and pay
    //the distributor
    virtual void effectTransfer(
            std::shared_ptr<Distributor> to) override
    {
        const Money benefit = getBenefit(to);
        to->addMoney(benefit);
    }
};


class SalesSideEffects::CompanyCommission
    : public EffectTransferable
{
public:
    CompanyCommission(
            const double _downstreamPercent,
            std::shared_ptr<Distributor> _company,
            std::shared_ptr<Consumer> _buyer,
            const Money _soldFor,
            const Money _wholesalePrice, 
            const BeneficiaryChain _chain)
        : EffectTransferable(
                _downstreamPercent,
                _company,
                _buyer, 
                _soldFor,
                _wholesalePrice,
                _chain)
    {}

    virtual ~CompanyCommission() {}

protected:
    virtual Money getBenefit(std::shared_ptr<Distributor> who) const override
    {
        auto findRes = std::find_if(getBeneficiaryChain().cbegin(), getBeneficiaryChain().cend(),
                [who](std::shared_ptr<Distributor> x){
                    return who->id == x->id;
                });
        if(findRes == getBeneficiaryChain().cend())
        {
            unknownBeneficiaryError(who);
        }
        else
        {
            std::shared_ptr<Distributor> foundDistributor = *findRes,
                originalSeller = getBeneficiaryChain().front();
            if(foundDistributor->id == originalSeller->id)
            {
                //original seller gets paid the value of the sale
                return getSoldForPrice();
            }
            else
            {
                //everyone else gets commission (paid by the company)
                return downstreamPercent * getSoldForPrice();
            }
        }
    }

    virtual void effectTransfer(
            std::shared_ptr<Distributor> to) override
    {
        const Money benefit = getBenefit(to);
        to->addMoney(benefit);

        //if this isn't the seller then the company is paying commission
        //(otherwise the consumer paid them)
        if((*to) != (*seller))
        {
            company->deductMoney(benefit);
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
        SalesSideEffects::BeneficiaryChain chain)
{
    auto currIt = chain.begin(),
        nextIt = chain.begin(),
        endIt = chain.end();

    ASSERT_WITH_MESSAGE(chain.size() >= 1, 
            "The beneficiary chain has to at least contain the seller");

    nextIt++;

    while(currIt != endIt)
    {
        //if there's no next then the current distributor shouldn't have a recruitedBy
        if(nextIt == endIt)
        {
            std::shared_ptr<Distributor> seller = *currIt;
            //the last beneficiary should always have been recruited by the company
            //(meaning the recruitedBy field is null)
            assert(seller->getRecruitedBy().get() == nullptr);
        }
        //otherwise, check they match up
        else
        {
            assert(wasRecruitedBy(*currIt, *nextIt));
        }

        currIt++;
        nextIt++;
    }
}


/*******************************/
/********ENTRY POINT************/
/*******************************/

void SalesSideEffects::apply(
    const bool companyPaysCommission,
    const double downstreamPercent,
    const Money wholesalePrice,
    std::shared_ptr<Distributor> company,
    const SaleHandler::RecordType& salesRecords)
{
    for(const auto& thisRecord : salesRecords.records)
    {
        SaleHandler::Sale* recPtr = thisRecord->getRightPtr();
        
        //ignore non-sales records
        if(recPtr != nullptr)
        {
            const Money soldFor = recPtr->price;
            const BeneficiaryChain chain = SalesSideEffects::getBeneficiaryChain(recPtr->seller);
            std::unique_ptr<EffectTransferable> formula;

            SalesSideEffects::auditBeneficiaryChain(chain);
            
            if(companyPaysCommission)
            {
                formula = make_unique<CompanyCommission>(
                        downstreamPercent,
                        company,
                        recPtr->buyer,
                        soldFor,
                        wholesalePrice,
                        chain);
            }
            else
            {
                formula = make_unique<ChainedPercentWithGuarantee>(
                        downstreamPercent,
                        company,
                        recPtr->buyer,
                        soldFor,
                        wholesalePrice,
                        chain);
            }

            formula->effectTransfers();
        }
    }
}

}

