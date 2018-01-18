#pragma once

#include "Actors.hpp"
#include "Util/Unique.hpp"
#include "Types.hpp"
#include "ai/EagerActors.hpp"

namespace pyramid_scheme_simulator {

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
