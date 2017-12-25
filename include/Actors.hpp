#pragma once

#include "Util/Unique.hpp"
#include "ChanceContributor.hpp"
#include "CapitalHolder.hpp"

#include <memory>

namespace pyramid_scheme_simulator {

class StaticConsumer : public Consumer
{
    std::unique_ptr<ChanceContributor> salesChance;
    std::unique_ptr<ChanceContributor> conversionChance;

protected:
    StaticConsumer(Unique, Money,
            std::unique_ptr<ChanceContributor>&&,
            std::unique_ptr<ChanceContributor>&&);

public:
    StaticConsumer(Unique, Money, 
            const double salesChance, const double conversionChance);

    StaticConsumer(Unique, Money,
            ChanceContributor*,
            ChanceContributor*);

    StaticConsumer(Unique, Money,
            std::unique_ptr<ChanceContributor>&,
            std::unique_ptr<ChanceContributor>&);

    //copy constructor
    StaticConsumer(const StaticConsumer&);

    virtual ~StaticConsumer() {}

    virtual std::shared_ptr<CapitalHolder> clone() const override;

    virtual std::shared_ptr<Distributor> 
        becomeDistributor(
                std::shared_ptr<Distributor> convertedBy) override;


    virtual ChanceContributor&
        getSalesChanceContribution() const override;

    virtual ChanceContributor&
        getDistributorConversionChanceContribution() const override;
};

class Company : public Distributor
{
protected:
    std::vector<Money> disbursements;
    virtual bool canPurchase(Money, const CapitalHolder&) override;

    static const Inventory inventory;
public:
    virtual void deductMoney(Money) override;
    virtual unsigned int getInventory() const override;

    virtual ~Company() {}
};

class StaticDistributor : public Distributor
{
    //make StaticConsumer a friend so it can turn itself into a StaticDistributor
    //using StaticDistributor(Consumer&, std::shared_ptr<Distributor>)
    friend StaticConsumer;

    std::unique_ptr<ChanceContributor> salesChance;

protected:
    const Inventory desiredRestockAmount;
    const Inventory restockThreshold;

    StaticDistributor(Consumer& self, std::shared_ptr<Distributor> convBy) 
        : Distributor(self, convBy), 
          desiredRestockAmount(Config::Defaults::defaultDesiredRestockAmount),
          restockThreshold(Config::Defaults::defaultRestockThreshold)
    {}
    //used by other constructors
    StaticDistributor(
            Unique, 
            Money, 
            Inventory, 
            Inventory,
            Inventory,
            std::unique_ptr<ChanceContributor>&&);

    static const std::unique_ptr<ChanceContributor> conversionChance;
public:
    StaticDistributor(Unique, Money, Inventory, Inventory, const double salesChance);
    StaticDistributor(Unique, Money, Inventory, Inventory, ChanceContributor*);
    StaticDistributor(Unique, Money, Inventory, Inventory, std::unique_ptr<ChanceContributor>&);

    //copy constructor
    StaticDistributor(const StaticDistributor&);

    virtual ~StaticDistributor();

    virtual ChanceContributor&
        getSalesChanceContribution() const override;

    virtual ChanceContributor&
        getDistributorConversionChanceContribution() const override;

    virtual Inventory getDesiredRestockAmount() const override;
    virtual Inventory getRestockThreshold() const override;

    virtual std::shared_ptr<CapitalHolder> clone() const override;
};

}
