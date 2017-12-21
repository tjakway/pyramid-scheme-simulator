#pragma once

#include <vector>

#include "Types.hpp"
#include "PopulationGraph.hpp"
#include "TransactionObjects.hpp"

namespace pyramid_scheme_simulator {


class SalesSideEffects
{
protected:
    using BeneficiaryChain = std::vector<std::shared_ptr<Distributor>>;
     
    BeneficiaryChain getBeneficiaryChain(const std::shared_ptr<Distributor>);
    void auditBeneficiaryChain(BeneficiaryChain, std::shared_ptr<Distributor> company);

    //maps a distributor to how much they get paid
    class BenefitFormula
    {
    protected:
        Money getSoldForPrice() const;
        Money getWholesalePrice() const;
        BeneficiaryChain getBeneficiaryChain() const;
    public:
        BenefitFormula(const Money,
                const Money, 
                const BeneficiaryChain);

        virtual Money getBenefit(std::shared_ptr<Distributor> who) const;
        virtual Money getBenefit(const Distributor& who) const = 0;

    private:
        const Money soldFor;
        const Money wholesalePrice;
        const BeneficiaryChain beneficiaryChain;
    };

    class ChainedPercentWithGuarantee;

public:
    static void apply(PopulationGraph&,
        SaleHandler::RecordType);
};

}
