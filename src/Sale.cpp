#include "TransactionRecords.hpp"
#include "TransactionObjects.hpp"

#include <memory>
#include <functional>
#include <string>

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

SaleHandler::MoneyChangeRecord checkBuyerRecord(SimulationTick when, 
        Money price, 
        Consumer* buyer)
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
        return SaleHandler::MoneyChangeRecord(when, price, buyer);
    }
}

SaleHandler::MoneyChangeRecord checkSellerRecord(
        SimulationTick when,
        Money price, 
        Distributor* seller, 
        Consumer* buyer)
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
        return SaleHandler::MoneyChangeRecord(when, price, seller);
    }
}

template <typename T>
std::string hashToStr(T t)
{
    std::hash<T> hasher;
    return std::to_string(hasher(t));
}

} // anonymous namespace



namespace pyramid_scheme_simulator {


bool UniqueRecord::operator==(const UniqueRecord& other)
{
    return when == other.when && id == other.id;
}

SaleHandler::MoneyChangeRecord::MoneyChangeRecord(SimulationTick when, Money price, 
        CapitalHolder* p)
    : UniqueRecord(when, p->id),
      fundsBefore(p->getMoney()), 
      fundsAfter(p->getMoney() - price)
{ }

Unique SaleHandler::Sale::getUnique(SimulationTick when, 
            Money price, 
            Distributor* seller,
            Consumer* buyer)
{
    std::hash<std::string> hasher;
    return Unique(Util::hashToArray(hasher(
            hashToStr(when) + 
            hashToStr(price) + 
            hashToStr(seller->id) + 
            hashToStr(buyer->id)
    )));
}

SaleHandler::Sale::Sale(SimulationTick when, Money price, 
        const std::shared_ptr<Distributor> seller, 
        const std::shared_ptr<Consumer> buyer) 

    : UniqueRecord(when, 
            Sale::getUnique(when, price, 
                seller.get(), buyer.get())),

      price(price), 
      sellerRecord(checkSellerRecord(when, price, seller.get(), buyer.get())),
      buyerRecord(checkBuyerRecord(when, price, buyer.get()))
{ }


bool SaleHandler::Sale::operator==(const Sale& other)
{
    return UniqueRecord::operator==(other);
}

}
