#include "BoundedOption.hpp"

#include <string>

namespace pyramid_scheme_simulator {

std::string BoundedOption::defaultErrorMessage()
{
    std::ostringstream ss;
    ss << "Option " << outer->getOption() << " not in range " <<
        outer->range.first << " to " << outer->range.second << 
        std::endl;
    return ss.str();
}

}
