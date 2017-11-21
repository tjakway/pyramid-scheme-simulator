#include "TransactionObjects.hpp"
#include "SalesResult.hpp"

#include <string>
#include <exception>
#include <memory>

namespace pyramid_scheme_simulator {

/**
 * this class exists to prevent dynamic_cast'ing twice
 * if the sale failed it just returns SalesResult,
 * otherwise it will return the downcasted pointers
 */
class SaleHandler::SaleIsPossibleResult
{
public:
    const SalesResult result;
    const std::shared_ptr<Distributor> seller;
    const std::shared_ptr<Consumer> buyer;
private:
    SaleIsPossibleResult(SalesResult r, 
            const std::shared_ptr<Distributor> seller,
            const std::shared_ptr<Consumer> buyer)
        : result(r), success(r.success), seller(seller), buyer(buyer)
    {}

public:
    class SaleIsPossibleResultException :  public std::exception {
        std::string msg;
    public:
        SaleIsPossibleResultException(SalesResult res) {
            std::ostringstream ss;
            ss << "Tried to access downcasted pointers in SaleIsPossibleResult"
                << " when SalesResult was " << res << std::endl; 
            msg = ss.str();
        }
        SaleIsPossibleResultException(std::string msg) : msg(msg) {}

        virtual const char* what() const throw() override { 
            return msg.c_str();
        }
    };

    explicit operator bool() const {
        return result.success;
    }

    bool operator!() const {
        return !result.success;
    }

    const bool success;


    static const std::shared_ptr<Distributor> 
        checkSellerPointer(std::shared_ptr<Distributor>);
    static const std::shared_ptr<Consumer> checkBuyerPointer(std::shared_ptr<Consumer>);


    //thin wrappers over the static methods
    const std::shared_ptr<Distributor> checkSellerPointer() {
        SaleIsPossibleResult::checkSellerPointer(seller);
    }
    const std::shared_ptr<Consumer> checkBuyerPointer() {
        SaleIsPossibleResult::checkBuyerPointer(buyer);
    }

    static SaleIsPossibleResult good(SalesResult r, 
            const std::shared_ptr<Distributor> seller,
            const std::shared_ptr<Consumer> buyer)
    {
        if(!r)
        {
            throw SaleIsPossibleResultException(
                    "SalesResult passed to Transactions::SaleIsPossibleResult::good \
                     was not SUCCESS");
        }
        else
        {
            return SaleIsPossibleResult(r, 
                    checkSellerPointer(seller), 
                    checkBuyerPointer(buyer));
        }
    }

    static SaleIsPossibleResult bad(SalesResult r)
    {
        if(r)
        {
            throw SaleIsPossibleResultException(
                    "SUCCESS was passed to Transactions::SaleIsPossibleResult::bad");
        }
        else
        {
            return SaleIsPossibleResult(r, nullptr, nullptr);
        }
    }

};

const std::shared_ptr<Distributor> 
    SalesHandler::SaleIsPossibleResult::checkSellerPointer(std::shared_ptr<Distributor> seller)

{
    if(seller.get() == nullptr)
    {
        throw SaleIsPossibleResultException(
                "Passed null Distributor pointer to SaleIsPossibleResult::good");
    }
    else
    {
        return seller;
    }
}

const std::shared_ptr<Consumer> 
    SaleHandler::SaleIsPossibleResult::checkBuyerPointer(std::shared_ptr<Consumer> buyer)
{
    if(buyer.get() == nullptr)
    {
        throw SaleIsPossibleResultException(
                "Passed null Consumer pointer to SaleIsPossibleResult::good");
    }
    else
    {
        return buyer;
    }
}

void SaleHandler::processPotentialRestocking(SimulationTick when,
        Money price,
        CapitalHolder& holder)
{
    if(holder.isDistributor())
    {
        Distributor& d = dynamic_cast<Distributor&>(holder);
        const auto desiredRestock = d.getDesiredRestockAmount();

        //TODO: process the sale but without sampling random chance
        //since the distributor has to restock
    }
}

SalesResult SaleHandler::processPotentialSale(
        SimulationTick when, 
        Money price,
        rd_ptr rd,
        CapitalHolder& seller,
        CapitalHolder& buyer)
{
    auto saleIsPossibleResult = saleIsPossible(seller, buyer);
    if(saleIsPossibleResult)
    {
        auto chanceProc = sampleSalesChance(rd, seller, buyer);
        if(chanceProc)
        {
            
            Sale s(when, price, 
                    saleIsPossibleResult.seller,
                    saleIsPossibleResult.buyer);
            //TODO: change to return a TransactionRecord
            sales.insert(s);
            return saleIsPossibleResult.result;
        }
        else
        {
            return chanceProc;
        }
    }
    else
    {
        return saleIsPossibleResult.result;
    }
}

SalesResult Transactions::sampleSalesChance(rd_ptr rd, CapitalHolder& seller, CapitalHolder& buyer)
{
    const bool saleOccurred = (seller.getSalesChanceContribution() + 
                buyer.getSalesChanceContribution())->sampleFrom(rd);
    if(saleOccurred)
    {
        return SalesResult(SalesResult::Reason::SUCCESS);
    }
    else
    {
        return SalesResult(SalesResult::Reason::CHANCE_FAILED);
    }
}



SaleHandler::SaleIsPossibleResult SaleHandler::saleIsPossible(
        CapitalHolder& _seller, CapitalHolder& _buyer)
{
    Distributor* seller = dynamic_cast<Distributor*>(&_seller);
    if(!seller) {
        return Transactions::SaleIsPossibleResult::bad(
                SalesResult(SalesResult::Reason::SELLER_NOT_DISTRIBUTOR));
    }
    else {
        Consumer* buyer = dynamic_cast<Consumer*> (&_buyer);
        if(!buyer) {
            return Transactions::SaleIsPossibleResult::bad(
                    SalesResult(SalesResult::Reason::BUYER_NOT_CONSUMER));
        }
        //make sure they're not also a distributor
        else if(buyer->isDistributor()){
            return Transactions::SaleIsPossibleResult::bad(
                    SalesResult(SalesResult::Reason::BOTH_DISTRIBUTORS));
        }
        else if(!seller->hasInventory()){
            return Transactions::SaleIsPossibleResult::bad(
                    SalesResult::Reason::NO_INVENTORY);
        }
        else {
            return Transactions::SaleIsPossibleResult::good(
                    SalesResult(SalesResult::Reason::SUCCESS),
                    std::shared_ptr<Distributor>(seller),
                    std::shared_ptr<Consumer>(buyer)
                    );
        }
    }
}


}
