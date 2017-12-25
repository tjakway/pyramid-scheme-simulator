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
     
    static BeneficiaryChain getBeneficiaryChain(const std::shared_ptr<Distributor>);
    static void auditBeneficiaryChain(BeneficiaryChain);
    static std::shared_ptr<Distributor> getSeller(const BeneficiaryChain&);

    //maps a distributor to how much they get paid
    class BenefitFormula
    {
    protected:
        Money getSoldForPrice() const;
        Money getWholesalePrice() const;
        const BeneficiaryChain& getBeneficiaryChain() const;

        NEW_EXCEPTION_TYPE(UnknownBeneficiaryException);
        [[noreturn]] void unknownBeneficiaryError(std::shared_ptr<Distributor>) const;

        BenefitFormula(const Money,
                const Money, 
                const BeneficiaryChain);

        virtual Money getBenefit(std::shared_ptr<Distributor> who) const = 0;

    private:
        const Money soldFor;
        const Money wholesalePrice;
        const BeneficiaryChain beneficiaryChain;
    };

    class EffectTransferable;

    //for when the company does *not* pay commission
    class ChainedPercentWithGuarantee;
    //for when it does
    class CompanyCommission;

public:
    static void apply(
        const bool,
        const double,
        const Money,
        PopulationGraph&,
        std::shared_ptr<Distributor>,
        SaleHandler::RecordType);
};

}
