#pragma once

#include <iostream>
#include <stdexcept>
#include <sstream>
#include <string>

#ifdef ASSERT_WITH_MESSAGE_NS
namespace ASSERT_WITH_MESSASGE_NS {
#endif

class AssertionFailedException : public std::runtime_error
{
public:
    AssertionFailedException(const std::string& what_arg)
        : std::runtime_error(what_arg)
    {}
};

#ifdef ASSERT_WITH_MESSAGE_NS
}
#endif

//see https://stackoverflow.com/questions/3767869/adding-message-to-assert
#ifndef NDEBUG
#   define ASSERT_WITH_MESSAGE(condition, message) \
    do { \
        if (! (condition)) { \
            std::ostringstream os; \
            os << "Assertion `" #condition "` failed in " << __FILE__ \
                      << " line " << __LINE__ << ": " << message << std::endl; \
            throw AssertionFailedException(os.str()); \
        } \
    } while (false)
#else
#   define ASSERT_WITH_MESSAGE(condition, message) do { } while (false)
#endif
