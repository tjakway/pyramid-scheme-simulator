#pragma once

namespace pyramid_scheme_simulator {

class StaticConsumer : public Consumer
{
    bool willBuy;
public:
    StaticConsumer(bool willBuy): willBuy(willBuy) {}

    /**
     * StaticConsumer ignores whoever we might be purchasing from
     */
    virtual bool willPurchase(const CapitalHolder const& from) {
        return willBuy;
    }
};

class ExpConsumer : public Consumer {
    /**
     * constant factor from 0-1 reflecting how much this consumer likes
     * the product in question
     */
    double innateLikelihood;
public:

};

}
