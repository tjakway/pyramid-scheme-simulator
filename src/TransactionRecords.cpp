#include "TransactionRecords.hpp"

#include <memory>
#include <functional>
#include <string>

namespace pyramid_scheme_simulator {



bool compareUniqueables(Uniqueable* fst,
        Uniqueable* snd)
{
    const bool fstEmpty = fst == nullptr;
    const bool sndEmpty = snd == nullptr;

    //check for null pointers
    if(fstEmpty && sndEmpty) {
        return false;
    }
    else if(!fstEmpty && sndEmpty) {
        return false;
    }
    else if(fstEmpty && !sndEmpty) {
        return true;
    }
    //neither pointer is null, dereference & compare
    else {
        return fst->id.str() < snd->id.str();
    }
}
}


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

MoneyChangeRecord checkBuyerRecord(SimulationTick when, 
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
        return MoneyChangeRecord(when, price, buyer);
    }
}

MoneyChangeRecord checkSellerRecord(
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
        return MoneyChangeRecord(when, price, seller);
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

MoneyChangeRecord::MoneyChangeRecord(SimulationTick when, Money price, 
        CapitalHolder* p)
    : UniqueRecord(when, p->id),
      fundsBefore(p->getMoney()), 
      fundsAfter(p->getMoney() - price)
{ }

Unique Sale::getUnique(SimulationTick when, 
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

Sale::Sale(SimulationTick when, Money price, 
        const std::shared_ptr<Distributor> seller, 
        const std::shared_ptr<Consumer> buyer) 
    : UniqueRecord(when, )
      price(price), 
      sellerRecord(checkSellerRecord(price, seller, buyer)),
      buyerRecord(checkBuyerRecord(price, buyer))
{ }


}
