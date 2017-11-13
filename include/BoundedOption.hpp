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

public:
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
        OptionNotInRangeException()
        {
            std::ostringstream ss;
            ss << "Option " << getOption() << " not in range " <<
                range.first() << " to " << range.second() << std::endl;
            msg = ss.str();
        }

        const char* what() override
        {
            return msg.c_str();
        }
    };

    BoundedOption(T opt, std::pair<T, T> range)
        : option(opt)
    {
        if(!(opt <= range.first() && opt >= range.second()))
        {
            throw OptionNotInRangeException(range);
        }
    }

    /**
     * constructor to pass the range in ahead of time
     */
    BoundedOption(std::pair<T, T> r)
        : range(r)
    { }

    bool hasOption() { return option.get() == nullptr; };
    virtual T getOption() { return *option; }
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
