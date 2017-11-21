#include "Util/Unique.hpp"
#include "Guid.hpp"

namespace pyramid_scheme_simulator {

static const Unique emptyUnique = Unique(xg::Guid());

const UniqueSet& UniqueSet::emptyUniqueSet = Unique::emptyUnique;

}
