#include "CapitalHolder.hpp"

#include <string>
#include <sstream>
#include <exception>

namespace pyramid_scheme_simulator {

void CapitalHolder::setMoney(Money m)
{
    money = m;
}

Money CapitalHolder::getMoney()
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

bool Distributor::canPurchase(Money cost, const CapitalHolder& from)
{
    if(*this != from)
    {
        return false;
    }
    else
    {
        //XXX
    }
}

void Distributor::addMoney(Money howMuch)
{
    setMoney(getMoney() + howMuch);
}

}
