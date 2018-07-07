#pragma once

#include <string>

class PrettyPrintable
{
public:
    virtual std::string prettyPrint() const = 0;
};
