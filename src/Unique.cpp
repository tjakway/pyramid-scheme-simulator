#include "Util/Unique.hpp"
#include "Guid.hpp"

namespace pyramid_scheme_simulator {

static const Unique emptyUnique = Unique(xg::Guid());

const UniqueSet& UniqueSet::emptyUniqueSet = Unique::emptyUnique;

std::ostream& operator<<(std::ostream &os, const Unique &unique)
{
    os << unique.id;
    return os;
}


bool Unique::operator<(const Unique& other) const
{
    return id < other.id;
}

}


//instantiate template specializations for Unique
namespace std {
size_t hash<pyramid_scheme_simulator::Unique>::operator()(pyramid_scheme_simulator::Unique const &  u) const
{
    hash<string> hasher;
    return hasher(u.str());
}

}
