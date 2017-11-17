#include "ChanceContributor.hpp"

#include "Types.hpp"
#include "Util.hpp"

namespace pyramid_scheme_simulator {


bool ChanceContributor::sampleFrom(rd_ptr rd,
        ChanceContributor& lhs,
        ChanceContributor& rhs)
{
    //intersection of both chances
    const auto chance = lhs.getChance(rd) * rhs.getChance(rd);
    return Util::sampleFrom(rd, chance);
}

StaticChanceContributor::StaticChanceContributor(double staticChance)
    : chance(staticChance) {}

/**
 * ignore the passed random device and return the chance we got in the CTOR
 */
double StaticChanceContributor::getChance(rd_ptr)
{
    return chance;
}

std::unique_ptr<ChanceContributor> StaticChanceContributor::clone()
{
    return make_unique<StaticChanceContributor>(chance);
}


}
