#include "CapitalHolder.hpp"

#include <string>
#include <sstream>
#include <exception>
#include <mutex>

namespace pyramid_scheme_simulator {


void CapitalHolder::atomicallyDoSaleSideEffects(
        Money price,
        Distributor* seller,
        Consumer* buyer)
{
    //use std::lock to lock 2 mutexes at the same time
    //see http://en.cppreference.com/w/cpp/thread/lock
    std::lock(seller->mutex, buyer->mutex);
    
    //take ownership of the locks
    std::lock_guard<std::mutex> lk1(seller->mutex, std::adopt_lock);
    std::lock_guard<std::mutex> lk2(buyer->mutex,  std::adopt_lock);

    //process the sale
    //the calling function should have checked all preconditions before
    //calling this
    buyer->incrementInventory();
    buyer->deductMoney(price);

    seller->decrementInventory();
    seller->addMoney(price);
}


void CapitalHolder::setMoney(Money m)
{
    money = m;
}

Money CapitalHolder::getMoney() const
{
    return money;
}


/**
 * compare unique IDs
 */
bool CapitalHolder::operator==(const CapitalHolder& other)
{
    return this->id == other.id;
}

bool CapitalHolder::operator!=(const CapitalHolder& other)
{
    return this->id != other.id;
}

void Consumer::deductMoney(Money howMuch)
{
    //should never happen
    class InsufficientFundsException : public std::exception
    {
        std::string msg;
    public:
        InsufficientFundsException(std::string m) : msg(m) {}
        const char* what() const throw() override {
            return msg.c_str();
        }
    };

    if(getMoney() <  howMuch)
    {
        std::stringstream ss;
        ss << "Cannot deduct " << howMuch <<
            " from account with only " << getMoney() <<
            std::endl;
        throw InsufficientFundsException(ss.str());
    }
    else
    {
        setMoney(getMoney() - howMuch);
    }
}

bool Consumer::canBecomeDistributor(Money buyIn) const
{
    return buyIn < getMoney();
}

Distributor::Distributor(Unique id, Money m, std::shared_ptr<Distributor> _recruitedBy)
    : Consumer(id, m),
    recruitedBy(_recruitedBy)
{ }

Distributor::Distributor(Unique id, Money m)
    : Distributor(id, m, nullptr)
{}


//initialize a distributor from a consumer
//metaphorically the consumer is being recruited as a distributor
//so the fields should match up
Distributor::Distributor(Consumer& from, std::shared_ptr<Distributor> convertedBy)
    : Distributor(from.id, 
            from.getMoney(), 
            std::shared_ptr<Distributor>(convertedBy))
{
    setInventory(from.getInventory());
}


std::shared_ptr<Distributor> Distributor::becomeDistributor(std::shared_ptr<Distributor>)
{
    throw AlreadyDistributorException(
            STRCAT("CapitalHolder with id ", id, " is already a distributor"));
}

bool Distributor::canPurchase(Money cost, const CapitalHolder& from)
{
    if(getMoney() < cost)
    {
        return false;
    }
    //can't buy from yourself
    else if(id == from.id)
    {
        return false;
    }
    else {
        return true;
    }
}

void Distributor::addMoney(Money howMuch)
{
    setMoney(getMoney() + howMuch);
}

}
