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
    StaticConsumer(StaticConsumer&);

    virtual ~StaticConsumer() {}

    virtual ChanceContributor&
        getSalesChanceContribution() override;

    virtual ChanceContributor&
        getDistributorConversionChanceContribution() override;
};

class Company : public Distributor
{
protected:
    virtual bool canPurchase(Money, const CapitalHolder&) override;

    static const Inventory inventory;
public:
    virtual void deductMoney(Money) override;
    virtual unsigned int getInventory() override;

    virtual ~Company() {}
};

class StaticDistributor : public Distributor
{
    std::unique_ptr<ChanceContributor> salesChance;
protected:
    StaticDistributor(Unique, Money, Inventory);
    StaticDistributor(Consumer& self, std::shared_ptr<Distributor> convBy) : Distributor(self, convBy)
    {}

    static const std::unique_ptr<ChanceContributor> conversionChance;
public:
    StaticDistributor(Unique, Money, Inventory, const double salesChance);
    StaticDistributor(Unique, Money, Inventory, ChanceContributor*);
    StaticDistributor(Unique, Money, Inventory, std::unique_ptr<ChanceContributor>&);

    virtual ~StaticDistributor() {}

    virtual ChanceContributor&
        getSalesChanceContribution() override;

    virtual ChanceContributor&
        getDistributorConversionChanceContribution() override;
};

}
