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

}
