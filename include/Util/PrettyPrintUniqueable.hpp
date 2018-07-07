#pragma once

#include "Util/PrettyPrintable.hpp"
#include "Util/Unique.hpp"

#include <string>

namespace pyramid_scheme_simulator {

class PrettyPrintUniqueable : public PrettyPrintable, public Uniqueable
{
public:
    virtual std::string prettyPrint() const override;

    /**
     * we want the class to be abstract but it doesn't actually need any pure virtual functions
     */
    virtual ~PrettyPrintUniqueable() = 0;
};

}
