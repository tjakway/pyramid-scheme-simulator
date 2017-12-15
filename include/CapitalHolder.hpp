#pragma once

#include <set>
#include <vector>
#include <functional>

#include "CapitalHolderClassDecls.hpp"
#include "Util/Unique.hpp"
#include "Util/Lockable.hpp"
#include "Tick.hpp"
#include "Config.hpp"
#include "ChanceContributor.hpp"

namespace pyramid_scheme_simulator {

class CapitalHolder : public Uniqueable, protected Lockable
{
protected:
    Money money;

    Inventory inventory = 0;

    double getMoneyToProductCostRatio();

public:
    void setMoney(Money);

protected:
    virtual bool canPurchase(Money productCost, const CapitalHolder& /*from*/) {
        return money >= productCost;
    }

    CapitalHolder(Unique id, Money startingFunds) 
        : Uniqueable(id), money(startingFunds) {}

    CapitalHolder(Money startingFunds) 
        : CapitalHolder(Unique::emptyUnique, startingFunds) {}

public:
    virtual ~CapitalHolder() {}

    virtual std::shared_ptr<CapitalHolder> clone() const = 0;

    virtual ChanceContributor&
        getSalesChanceContribution() const = 0;

    virtual ChanceContributor&
        getDistributorConversionChanceContribution() const = 0;

    virtual bool isDistributor() const { return false; }

    Money getMoney() const;

    /** both Consumers and Distributors have inventory
     * but do different things with it */
    virtual Inventory getInventory() const {
        return inventory;
    }

    virtual void setInventory(Inventory i) {
        inventory = i;
    }

    virtual void incrementInventory() {
        inventory++;
    }

    virtual void decrementInventory() {
        inventory--;
    }

    bool operator==(const CapitalHolder&);
    bool operator!=(const CapitalHolder&);

    static void atomicallyDoSaleSideEffects(
            Money price,
            Distributor* seller,
            Consumer* buyer);
};

class Consumer : public CapitalHolder
{
public:
    Consumer(Unique id, Money startingFunds): CapitalHolder(id, startingFunds) {}
    virtual ~Consumer() {}

    virtual void deductMoney(Money);

    virtual std::shared_ptr<Distributor> 
        becomeDistributor(
                std::shared_ptr<Distributor> convertedBy) = 0;

    virtual bool canBecomeDistributor(Money buyIn) const;
};

class Distributor : public Consumer
{
protected:
    /**
     * who this distributor was recruited by
     * nullptr if they directly bought in from the company
     */
    std::shared_ptr<Distributor> recruitedBy;
    virtual bool canPurchase(Money cost, const CapitalHolder& from) override;

    bool isSubDistributor() { return recruitedBy.get() != nullptr; }

    Distributor(Unique, Money, std::shared_ptr<Distributor>);
    Distributor(Unique, Money);

    //initialize from a consumer
    //used in becomeDistributor
    Distributor(Consumer&, std::shared_ptr<Distributor>);

public:
    virtual ~Distributor() {}

    void addMoney(Money);

    virtual bool canBecomeDistributor(Money /*buyIn*/) const override { return false; }

    virtual bool isDistributor() const override { return true; }

    virtual Inventory getDesiredRestockAmount() const = 0;

    virtual bool hasInventory() const { return getInventory() > 0; }

    virtual Inventory getRestockThreshold() = 0;
    virtual bool needsRestock() { return getInventory() > getRestockThreshold(); }
};


} //pyramid_scheme_simulator
