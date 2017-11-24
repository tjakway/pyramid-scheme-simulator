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

public:
    void setOption(T opt) 
    {
        option = std::unique_ptr<T>(opt);
        checkOption();
    }

protected:
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
            throw OptionNotInRangeException(optionNotInRangeMsg);
        }
        else
        {
            return *option;
        }
    }

public:

    bool hasOption() { return option.get() == nullptr; };
    virtual T getOption() { return checkOption(); }

    BoundedOption(T* opt, std::pair<T, T> range, std::string onErrorMsg)
        : option(std::unique_ptr<T>(opt)), range(range), optionNotInRangeMsg(onErrorMsg)
    {
        if(opt != nullptr && !(*opt <= range.first && *opt >= range.second))
        {   
            throw BoundedOption::OptionNotInRangeException(onErrorMsg);
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
