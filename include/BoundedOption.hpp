#pragma once

#include <utility>
#include <exception>
#include <stdexcept>
#include <string>
#include <sstream> 
#include <memory>

#include "Util/Util.hpp"
#include "Util/NewExceptionType.hpp"

namespace pyramid_scheme_simulator {

template<typename T>
class BoundedOption
{
public:
    /**
     * ERROR CLASSES
     */


    NEW_EXCEPTION_TYPE_WITH_BASE(BoundedOptionException, std::runtime_error);
    NEW_EXCEPTION_TYPE_WITH_BASE(OptionNotInRangeException, BoundedOptionException);
    //thrown if the BoundedOption was only given a range
    NEW_EXCEPTION_TYPE_WITH_BASE(OptionNotSetException, BoundedOptionException);

private:
    std::unique_ptr<T> option;
    std::pair<T, T> range;

public:
    void setOption(T opt) 
    {
        option = make_unique<T>(opt);
        checkOption();
    }

protected:
    std::pair<T, T> getRange() { return range; }

    void setRange(std::pair<T, T> r) { range = r; }

    static std::string defaultErrorMessage(std::pair<T, T> paramRange)
    {
        std::ostringstream ss;
        ss << "Option not in range " <<
            paramRange.first << " to " << paramRange.second << 
            std::endl;
        return ss.str();
    }

    std::string optionNotInRangeMsg;

    void rangeCheck()
    {
        if((*option < range.first) || (*option > range.second))
        {
            throw OptionNotInRangeException(optionNotInRangeMsg);
        }
    }

    T checkOption()
    {
        if(option == nullptr)
        {
            throw OptionNotSetException("BoundedOption was only given a range");
        }
        else
        {
            rangeCheck();
            return *option;
        }
    }

public:

    bool hasOption() { return option.get() == nullptr; };
    virtual T getOption() { return checkOption(); }

protected:
    /**
     * passed type must be copy-constructable
     */
    BoundedOption(T* opt, std::pair<T, T> _range, std::string onErrorMsg)
        : range(_range), optionNotInRangeMsg(onErrorMsg)
    {
        //don't throw if the option isn't set yet
        if(opt != nullptr)
        {
            setOption(*opt);
            checkOption();
        }
    }

public:

    BoundedOption(T& opt, std::pair<T, T> _range, std::string onErrorMsg)
        : BoundedOption(&opt, _range, onErrorMsg)
    {}

    BoundedOption(T& opt, std::pair<T, T> _range)
        : BoundedOption(&opt, _range, defaultErrorMessage(_range))
    {}

    /**
    * constructor to pass the range in ahead of time
    */
    BoundedOption(std::pair<T, T> range)
        : BoundedOption(nullptr, range, defaultErrorMessage(range))
    { }



    BoundedOption(std::pair<T, T> range, std::string onErrorMsg)
        : BoundedOption(nullptr, range, onErrorMsg)
    { }
};

class PercentOption : public BoundedOption<double>
{
public:
    PercentOption(double pc) : BoundedOption(pc,
            std::pair<double, double>(0.0, 1.0))
        {}

    virtual ~PercentOption() {}
};

}
