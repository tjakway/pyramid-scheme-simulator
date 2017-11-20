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

    static const unsigned int inventory;
public:
    virtual void deductMoney(Money) override;
    virtual unsigned int getInventory() override;
};

}
