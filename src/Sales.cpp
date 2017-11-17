#include "Sales.hpp"

#include <exception>
#include <string>
#include <sstream>

namespace {

using namespace pyramid_scheme_simulator;
class InvalidSale : public std::exception
{
    std::string msg;
public:
    InvalidSale(std::string m) : msg(m) {}

    virtual const char* what() const throw() override
    { return msg.c_str(); }
};

MoneyChangeRecord checkBuyerRecord(Money price, const std::shared_ptr<Consumer> buyer)
{
    if(buyer->getMoney() < price)
    {
        std::ostringstream ss;
        ss << "Insufficient funds for sale to Consumer with ID "
            << buyer->id << "." << std::endl
            << "Expected >= " << price << ", but only have "
            << buyer->getMoney() << std::endl;
        throw InvalidSale(ss.str());
    }
    else
    {
        return MoneyChangeRecord(price, buyer);
    }
}

MoneyChangeRecord checkSellerRecord(Money price, 
        const std::shared_ptr<Distributor> seller, 
        const std::shared_ptr<Consumer> buyer)
{
    //a distributor can't buy his own products (already paid the buy-in)
    if(*seller == *buyer)
    {
        std::ostringstream ss;
        ss << "A distributor cannot sell to himself (id: "
            << seller->id << ")." << std::endl;
        throw InvalidSale(ss.str());
    }
    else
    {
        return MoneyChangeRecord(price, seller);
    }
}

}

namespace pyramid_scheme_simulator {

SalesResult::SalesResult(Reason r)
    : reason(r), success(r == SUCCESS) 
{}

MoneyChangeRecord::MoneyChangeRecord(Money price, 
        const std::shared_ptr<CapitalHolder> p)
    : who(p->id), fundsBefore(p->getMoney()), fundsAfter(p->getMoney() - price)
{ }

Sale::Sale(SimulationTick when, Money price, 
        const std::shared_ptr<Distributor> seller, 
        const std::shared_ptr<Consumer> buyer) 
    : when(when), price(price), 
      sellerRecord(checkSellerRecord(price, seller, buyer)),
      buyerRecord(checkBuyerRecord(price, buyer))
{ }


/**
 * this class exists to prevent dynamic_cast'ing twice
 * if the sale failed it just returns SalesResult,
 * otherwise it will return the downcasted pointers
 */
class Transactions::SaleIsPossibleResult
{
private:
    const std::shared_ptr<Distributor> seller;
    const std::shared_ptr<Consumer> buyer;

    SalesResult result;


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


    const std::shared_ptr<Distributor> checkSellerPointer();
    const std::shared_ptr<Consumer> checkBuyerPointer();

    static SaleIsPossibleResult good(SalesResult r, 
            const std::shared_ptr<Distributor> seller,
            const std::shared_ptr<Consumer> buyer)
    {
        if(!r)
        {
            throw SaleIsPossibleResultException(
                    "SalesResult passed to Transactions::SaleIsPossibleResult::good" +
                    " was not SUCCESS");
        }
        else
        {
            return SaleIsPossibleResult(r, 
                    checkSellerPointer(), 
                    checkBuyerPointer());
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

const std::shared_ptr<Distributor> Transactions::SaleIsPossibleResult::checkSellerPointer()

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

const std::shared_ptr<Consumer> Transactions::SaleIsPossibleResult::checkBuyerPointer()
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
            return Sale(when, 
                    price, 
                    seller, 
                    buyer);
        }
        else
        {
            return chanceProc;
        }
    }
    else
    {
        return isSalePossible;
    }
}

bool Transactions::sampleSalesChance(rd_ptr rd, CapitalHolder& seller, CapitalHolder& buyer)
{
    const bool saleOccurred = (seller.getSalesChanceContribution() + 
                buyer.getSalesChanceContribution())->sampleFrom(rd);
    return saleOccurred;
}

}
