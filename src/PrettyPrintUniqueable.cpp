#include "Util/PrettyPrintUniqueable.hpp"

#include "Util/Strcat.hpp"

#include <typeinfo>

namespace pyramid_scheme_simulator {

/**
 * see http://www.gotw.ca/gotw/031.htm regarding pure virtual destructors
 */
PrettyPrintUniqueable::~PrettyPrintUniqueable() {}


std::string PrettyPrintUniqueable::prettyPrint() const
{
    //see https://stackoverflow.com/questions/1024648/retrieving-a-c-class-name-programmatically
    const std::string concreteClassName = typeid(*this).name();
    return STRCAT("[", concreteClassName, " ", prettyPrintId(), "]");
}

}
