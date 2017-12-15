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
    virtual bool canPurchase(Money, const CapitalHolder&) override;

    static const Inventory inventory;
public:
    virtual void deductMoney(Money) override;
    virtual unsigned int getInventory() const override;

    virtual ~Company() {}
};

class StaticDistributor : public Distributor
{
    std::unique_ptr<ChanceContributor> salesChance;
protected:
    StaticDistributor(Unique, Money, Inventory);
    StaticDistributor(Consumer& self, std::shared_ptr<Distributor> convBy) 
        : Distributor(self, convBy)
    {}
    //used by other constructors
    StaticDistributor(Unique, Money, Inventory, std::unique_ptr<ChanceContributor>&&);

    static const std::unique_ptr<ChanceContributor> conversionChance;
public:
    StaticDistributor(Unique, Money, Inventory, const double salesChance);
    StaticDistributor(Unique, Money, Inventory, ChanceContributor*);
    StaticDistributor(Unique, Money, Inventory, std::unique_ptr<ChanceContributor>&);

    //copy constructor
    StaticDistributor(const StaticDistributor&);

    virtual ~StaticDistributor();

    virtual ChanceContributor&
        getSalesChanceContribution() const override;

    virtual ChanceContributor&
        getDistributorConversionChanceContribution() const override;

    virtual std::shared_ptr<CapitalHolder> clone() const override;
};

}
