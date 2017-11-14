#pragma once

#include <utility>
#include <exception>
#include <string>
#include <sstream> 
#include <memory>

namespace pyramid_scheme_simulator {

template<typename T>
class BoundedOption
{
private:
    std::unique_ptr<T> option;
    std::pair<T, T> range;

protected:
    void setOption(T opt) 
    {
        option = std::unique_ptr<T>(opt);
    }

    std::pair<T, T> getRange() { return range; }

    void setRange(std::pair<T, T> r) { range = r; }

    std::string defaultErrorMessage();
    std::string optionNotInRangeMsg = defaultErrorMessage();

public:
    class OptionNotSetException;

    bool hasOption() { return option.get() == nullptr; };
    virtual T getOption() 
    { 
        if(option == nullptr)
        {
            throw OptionNotSetException();
        }
        else
        {
            return *option;
        }
    }

    class BoundedOptionException : public std::exception
    { };

    /**
     * exception thrown if the option is out of the specified bounds
     */
    class OptionNotInRangeException : 
        public BoundedOptionException
    {
        std::string msg;
    public:
        OptionNotInRangeException(std::string explicitMsg)
            : msg(explicitMsg)
        { }

        virtual const char* what() const throw() override
        {
            return msg.c_str();
        }
    };

    class OptionNotSetException 
        : public BoundedOptionException
    {
        virtual const char* what() const throw() override
        {
            return "BoundedOption was only given a range.";
        }
    };

    /**
    * constructor to pass the range in ahead of time
    */
    BoundedOption(std::pair<T, T> r)
        : range(r)
    { }


    BoundedOption(std::pair<T, T> range, std::string onErrorMsg)
        : BoundedOption(range), optionNotInRangeMsg(onErrorMsg)
    { }


    BoundedOption(T opt, std::pair<T, T> range, std::string onErrorMsg)
        : BoundedOption(range), option(new T(opt))
    {
        if(!(opt <= range.first && opt >= range.second))
        {   
            throw BoundedOption::OptionNotInRangeException(onErrorMsg);
        }
    }


    BoundedOption(T opt, std::pair<T, T> range)
        : BoundedOption(opt, range, defaultErrorMessage())
    { }
};

class PercentOption : BoundedOption<double>
{
private:
    double option;

public:
    PercentOption(double pc) : BoundedOption(pc, std::pair<double, double>(0.0, 1.0)),
        option(pc) {}

    double getOption() override
    {
        return option;
    }
};

}