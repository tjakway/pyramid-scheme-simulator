#pragma once

#include <string>
#include <sstream>


//stringification macros
//see https://stackoverflow.com/questions/2653214/stringification-of-a-macro-value
//DON'T name it str() or there WILL be name conflicts!
//any name that no one would write in a regular C/C++ file will do...
#define XSTRINGIFY_MACRO__(a) STRINGIFY_MACRO__(a)
#define STRINGIFY_MACRO__(a) #a


//NOTE: the base class must have a constructor that takes a std::string
#define NEW_EXCEPTION_TYPE(A, B) \
    class A : public B \
    { \
        const std::string msg;\
    public: \
        virtual std::string getExceptionTypeName() { return XSTRINGIFY_MACRO__(A); } \
            \
        A(const std::string& message) \
            : B(message), msg(message)\
        {} \
        \
        virtual const char* what() const throw() override \
        { \
            std::ostringstream os;\
            os << "Exception of type " << \
            getExceptionTypeName() << \
            " thrown: " << msg << std::endl;\
            return os.str().c_str();\
        }\
    };
