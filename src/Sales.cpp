#include "Sales.hpp"

#include <exception>
#include <string>
#include <sstream>

namespace {

class InvalidSale : public std::exception
{
    std::string msg;
public:
    InvalidSale(std::string m) : msg(m) {}

    virtual const char* what() const throw() override
    { return msg.c_str(); }
};

}

namespace pyramid_scheme_simulator {

MoneyChangeRecord::MoneyChangeRecord(Money price, 
        const std::shared_ptr<CapitalHolder> p)
    : who(p->id), fundsBefore(p->getMoney()), fundsAfter(p->getMoney() - price)
{ }

Sale::Sale(SimulationTick when, Money price, 
        const std::shared_ptr<Distributor> seller, 
        const std::shared_ptr<Consumer> buyer) : when(when), price(price)
{
    const auto checkFunds = [&](const std::shared_ptr<CapitalHolder> p) {
        if(p->getMoney() < price)
        {
            std::ostringstream ss;
            ss << "Insufficient funds for sale to Consumer with ID "
                << p->id << "." << std::endl
                << "Expected >= " << price << ", but only have "
                << p->getMoney() << std::endl;
            throw InvalidSale(ss.str());
        }
    };

    //a distributor can't buy his own products (already paid the buy-in)
    if(*seller == *buyer)
    {
        std::ostringstream ss;
        ss << "A distributor cannot sell to himself (id: "
            << seller->id << ")." << std::endl;
        throw InvalidSale(ss.str());
    }

    checkFunds(buyer);


}

SalesResult Transactions::sell(const CapitalHolder& seller, const CapitalHolder& buyer)
{

}

}
