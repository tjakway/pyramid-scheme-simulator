#pragma once

#include <string>

class PrettyPrintable
{
public:
    virtual std::string prettyPrint() const = 0;

    /**
     * standardize pretty print formatting
     */
    std::string surroundName(std::string str) const
    {
        return "[" + str + "]";
    }
};
