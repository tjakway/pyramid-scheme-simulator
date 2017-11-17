#include "SalesRecords.hpp"

#include <exception>
#include <string>
#include <sstream>
#include <initializer_list>


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

/** CapitalHolderRecord *****/

CapitalHolderRecord::CapitalHolderRecord(const SimulationTick when,
        std::initializer_list<Unique> uniqs)
    : when(when)
{
    for(auto u: uniqs)
    {
        who.insert(u);
    }
}

std::unordered_set<Unique> CapitalHolderRecord::getWho()
{
    return who;
}

SimulationTick CapitalHolderRecord::getWhen()
{
    return when;
}

/**************************/

std::ostream& operator<<(std::ostream& os, const SalesResult& res)
{
    os << res.str();
    return os;
}

SalesResult::SalesResult(Reason r)
    : reason(r), success(r == SUCCESS) 
{}

MoneyChangeRecord::MoneyChangeRecord(SimulationTick when, Money price, 
        const std::shared_ptr<CapitalHolder> p)
    : CapitalHolderRecord(when, p->id),
      fundsBefore(p->getMoney()), 
      fundsAfter(p->getMoney() - price)
{ }

Sale::Sale(SimulationTick when, Money price, 
        const std::shared_ptr<Distributor> seller, 
        const std::shared_ptr<Consumer> buyer) 
    : CapitalHolderRecord(when,
            {checkSellerRecord(price, seller, buyer)->id,
             checkBuyerRecord(price, buyer)->id})
      price(price), 
      sellerRecord(checkSellerRecord(price, seller, buyer)),
      buyerRecord(checkBuyerRecord(price, buyer))
{ }


}
