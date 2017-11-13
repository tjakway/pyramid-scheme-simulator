#pragma once

#include <utility>
#include <exception>
#include <string>
#include <sstream> 

namespace pyramid_scheme_simulator {

template<typename T>
class BoundedOption
{
    T option;
    //no need to store the range since it's only used in the constructor

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
        OptionNotInRangeException(std::pair<T, T> range)
        {
            std::ostringstream ss;
            ss << "Option " << option << " not in range " <<
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

    T getOption() { return option; }
};

}
