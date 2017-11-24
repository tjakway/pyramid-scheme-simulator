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

    virtual Inventory getDesiredRestockAmount() override {
        return 5;
    }

    virtual Inventory getRestockThreshold() override {
        return 1;
    }
};

class EagerTestDistributor : public EagerDistributor
{
public:
    EagerTestDistributor(Money m, Inventory i = 0)
        : EagerDistributor(Unique::newUnique(), m, i)
    {}
};

}
