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
public:
    /**
     * ERROR CLASSES
     */

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
private:
    std::unique_ptr<T> option;
    std::pair<T, T> range;

protected:
    void setOption(T opt) 
    {
        option = std::unique_ptr<T>(opt);
        checkOption();
    }

    std::pair<T, T> getRange() { return range; }

    void setRange(std::pair<T, T> r) { range = r; }

    std::string defaultErrorMessage()
    {
        std::ostringstream ss;
        ss << "Option " << getOption() << " not in range " <<
            range.first << " to " << range.second << 
            std::endl;
        return ss.str();
    }

    std::string optionNotInRangeMsg = defaultErrorMessage();

    T checkOption()
    {
        if(option == nullptr)
        {
            throw OptionNotSetException();
        }
        else if(!(*option <= range.first && *option >= range.second))
        {
            throw OptionNotInRangeException();
        }
        else
        {
            return *option;
        }
    }

public:

    bool hasOption() { return option.get() == nullptr; };
    virtual T getOption() { return checkOption(); }


    /**
    * constructor to pass the range in ahead of time
    */
    BoundedOption(std::pair<T, T> r)
        : range(r)
    { }


    BoundedOption(std::pair<T, T> range, std::string onErrorMsg)
        : BoundedOption(range, optionNotInRangeMsg(onErrorMsg))
    { }


    BoundedOption(T opt, std::pair<T, T> range, std::string onErrorMsg)
        : BoundedOption(range, option(new T(opt)))
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
