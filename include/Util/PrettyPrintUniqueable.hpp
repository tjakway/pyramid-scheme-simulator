#pragma once

#include "Util/PrettyPrintable.hpp"
#include "Util/Unique.hpp"

#include <string>

#ifdef PRETTY_PRINTABLE_UTIL_NS
namespace PRETTY_PRINTABLE_UTIL_NS {
#endif

class PrettyPrintUniqueable : public PrettyPrintable, public Uniqueable
{
protected:
    PrettyPrintUniqueable() 
        : Uniqueable()
    {}

    PrettyPrintUniqueable(Unique x)
        : Uniqueable(x)
    {}

public:
    virtual std::string prettyPrintImpl() const override;

    /**
     * we want the class to be abstract but it doesn't actually need any pure virtual functions
     */
    virtual ~PrettyPrintUniqueable() = 0;
};

#ifdef PRETTY_PRINTABLE_UTIL_NS
}
#endif
