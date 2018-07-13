#pragma once

#include "NamespaceDefines.hpp"
#include "Actors.hpp"
#include "Util/Unique.hpp"
#include "Types.hpp"

BEGIN_PYRAMID_NAMESPACE

class EagerConsumer : public StaticConsumer
{
public:
    EagerConsumer(Unique u, Money m)
        : StaticConsumer(u, m, 1.0, 1.0)
    {}
    virtual ~EagerConsumer() {}
};


class EagerDistributor : public StaticDistributor
{
public:
    /**
     * pass 1.0 for both sales and conversion chance contributions because we're eager
     */
    EagerDistributor(Unique u, 
            Money startingMoney, 
            Inventory startingInventory, 
            Inventory desiredRestockAmount,
            Inventory restockThreshold)
        : StaticDistributor(u, startingMoney, 
                startingInventory, 
                desiredRestockAmount,
                restockThreshold,
                1.0, 1.0)
    {}

    EagerDistributor(Consumer& self, std::shared_ptr<Distributor> convBy) 
        : StaticDistributor(self, convBy)
    {}

    EagerDistributor(const EagerDistributor& other)
        : EagerDistributor(other.id, 
                other.getMoney(), 
                other.getInventory(),
                other.getDesiredRestockAmount(),
                other.getRestockThreshold())
    {}

    virtual ~EagerDistributor() {}

    virtual Inventory getDesiredRestockAmount() const override {
        return 5;
    }

    virtual Inventory getRestockThreshold() const override {
        return 1;
    }
};

END_PYRAMID_NAMESPACE
