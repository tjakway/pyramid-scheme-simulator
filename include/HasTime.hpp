#pragma once

#include "NamespaceDefines.hpp"
#include "Tick.hpp"

BEGIN_PYRAMID_NAMESPACE

class HasTime
{
protected:
    virtual SimulationTick when() const = 0;
};

END_PYRAMID_NAMESPACE
