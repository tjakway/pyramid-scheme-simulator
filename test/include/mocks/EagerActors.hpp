#pragma once

#include "Actors.hpp"
#include "Util/Unique.hpp"
#include "Types.hpp"

namespace pyramid_scheme_simulator {

class EagerConsumer : public StaticConsumer
{
public:
    EagerConsumer(Unique u, Money m)
        : StaticConsumer(u, m, 1.0, 1.0)
    {}
    virtual ~EagerConsumer() {}
};

/**
 * EagerConsumer with a random UUID
 */
class EagerTestConsumer : public EagerConsumer
{
public:
    EagerTestConsumer(Money m)
        : EagerConsumer(Unique::newUnique(), m)
    {}
    virtual ~EagerTestConsumer() {}
};

class EagerDistributor : public StaticDistributor
{
public:
    EagerDistributor(Unique u, Money m, Inventory i)
        : StaticDistributor(u, m, i, 1.0)
    {}

    EagerDistributor(Consumer& self, std::shared_ptr<Distributor> convBy) 
        : StaticDistributor(self, convBy)
    {}

    EagerDistributor(const EagerDistributor& other)
        : EagerDistributor(other.id, other.getMoney(), other.getInventory())
    {}

    virtual ~EagerDistributor() {}

    virtual Inventory getDesiredRestockAmount() const override {
        return 5;
    }

    virtual Inventory getRestockThreshold() const override {
        return 1;
    }
};

class EagerTestDistributor : public EagerDistributor
{
public:
    EagerTestDistributor(Money m, Inventory i = 0)
        : EagerDistributor(Unique::newUnique(), m, i)
    {}

    EagerTestDistributor(Consumer& self, std::shared_ptr<Distributor> convBy) 
        : EagerDistributor(self, convBy)
    {}

    EagerTestDistributor(const EagerTestDistributor& other)
        : EagerTestDistributor(other.getMoney(), other.getInventory())
    {}

    virtual std::shared_ptr<CapitalHolder> clone() const override
    {
        //call copy constructor
        return std::make_shared<EagerTestDistributor>(*this);
    }

    virtual ~EagerTestDistributor() {}
};

}
