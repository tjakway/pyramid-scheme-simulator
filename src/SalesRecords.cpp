#include "SalesRecords.hpp"

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


}
