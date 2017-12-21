#pragma once

#include <functional>

#include "PopulationGraph.hpp"
#include "TransactionObjects.hpp"

namespace pyramid_scheme_simulator {


class SalesSideEffects
{
protected:
    using BeneficiaryChain = std::vector<std::shared_ptr<Distributor>>;
     
    BeneficiaryChain getBeneficiaryChain(const std::shared_ptr<Distributor>);
    void auditBeneficiaryChain(BeneficiaryChain);

    //maps a distributor to how much they get paid
    class BenefitFormula
    {
    public:
        BenefitFormula(const Money soldFor,
                const Money wholesalePrice, 
                const BeneficiaryChain);

        virtual Money getBenefit(std::shared_ptr<Distributor> who) const = 0;
        virtual Money getBenefit(const Distributor& who) const;
    };

public:
    static void apply(PopulationGraph&,
        SaleHandler::RecordType);
};

}
