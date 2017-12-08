#pragma once

#include <utility>
#include <exception>
#include <stdexcept>
#include <string>
#include <sstream> 
#include <memory>

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
        option = std::unique_ptr<T>(new T(opt));
        checkOption();
    }

protected:
    std::pair<T, T> getRange() { return range; }

    void setRange(std::pair<T, T> r) { range = r; }

    std::string defaultErrorMessage()
    {
        std::ostringstream ss;
        ss << "Option not in range " <<
            range.first << " to " << range.second << 
            std::endl;
        return ss.str();
    }

    std::string optionNotInRangeMsg = defaultErrorMessage();

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

    BoundedOption(T* opt, std::pair<T, T> range, std::string onErrorMsg)
        : option(std::unique_ptr<T>(opt)), range(range), optionNotInRangeMsg(onErrorMsg)
    {
        //don't throw if the option isn't set yet
        if(opt != nullptr) {
            checkOption();
        }
    }

    /**
    * constructor to pass the range in ahead of time
    */
    BoundedOption(std::pair<T, T> range)
        : BoundedOption(nullptr, range, defaultErrorMessage())
    { }



    BoundedOption(std::pair<T, T> range, std::string onErrorMsg)
        : BoundedOption(nullptr, range, onErrorMsg)
    { }




    BoundedOption(T* opt, std::pair<T, T> range)
        : BoundedOption(opt, range, defaultErrorMessage())
    { }
};

class PercentOption : public BoundedOption<double>
{
public:
    PercentOption(double pc) : BoundedOption(new double(pc),
            std::pair<double, double>(0.0, 1.0))
        {}
};

}
