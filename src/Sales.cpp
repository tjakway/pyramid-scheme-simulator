#include "Sales.hpp"
#include "SalesRecords.hpp"

#include <exception>
#include <string>
#include <sstream>


namespace pyramid_scheme_simulator {


/**
 * this class exists to prevent dynamic_cast'ing twice
 * if the sale failed it just returns SalesResult,
 * otherwise it will return the downcasted pointers
 */
class Transactions::SaleIsPossibleResult
{
public:
    const SalesResult result;
private:
    const std::shared_ptr<Distributor> seller;
    const std::shared_ptr<Consumer> buyer;


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
    Transactions::SaleIsPossibleResult::checkSellerPointer(std::shared_ptr<Distributor> seller)

{
    if(!seller)
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
    Transactions::SaleIsPossibleResult::checkBuyerPointer(std::shared_ptr<Consumer> buyer)
{
    if(!buyer)
    {
        throw SaleIsPossibleResultException(
                "Passed null Consumer pointer to SaleIsPossibleResult::good");
    }
    else
    {
        return buyer;
    }
}

void processPotentialRestocking(SimulationTick when,
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

SalesResult Transactions::processPotentialSale(
        SimulationTick when, 
        Money price,
        rd_ptr rd,
        CapitalHolder& seller,
        CapitalHolder& buyer)
{
    auto isSalePossible = saleIsPossible(seller, buyer);
    if(isSalePossible)
    {
        auto chanceProc = sampleSalesChance(rd, seller, buyer);
        if(chanceProc)
        {
            Sale s(when, price, seller, buyer);
            sales.insert(s);
            return s;
        }
        else
        {
            return chanceProc;
        }
    }
    else
    {
        return isSalePossible.result;
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



Transactions::SaleIsPossibleResult Transactions::saleIsPossible(
        CapitalHolder& seller, CapitalHolder& buyer)
{
    //TODO: more checks
    return buyer.getInventory() > 0;
}



}
