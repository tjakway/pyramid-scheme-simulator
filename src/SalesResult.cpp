#include "SalesResult.hpp"

#include <sstream>

namespace pyramid_scheme_simulator {

std::ostream& operator<<(std::ostream& os, const SalesResult& res)
{
    os << res.str();
    return os;
}

SalesResult::SalesResult(Reason r)
    : reason(r), success(r == SUCCESS) 
{}

}
