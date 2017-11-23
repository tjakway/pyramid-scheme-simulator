#pragma once

#include <memory>
#include <stdexcept>
#include <string>
#include <sstream>

#ifndef EITHER_HPP_DEMANGLE_NAMES

/* Test for GCC > 3.0.0  and libstdc++ */
#if __GNUC__ > 3 && defined __GLIBCPP__
#define EITHER_HPP_DEMANGLE_NAMES
#endif

#endif

namespace {
#ifdef EITHER_HPP_DEMANGLE_NAMES
//see https://gcc.gnu.org/onlinedocs/libstdc++/libstdc++-html-USERS-4.3/a01696.html
#include <cxxabi.h>
#include <cstdlib>

class DemanglingException : public std::runtime_error
{
public:
    DemanglingException(const std::string& what_arg)
        : std::runtime_error(what_arg)
    {}
};

std::string demangle(const char* mangledName)
{
    std::size_t length;
    int status;

    char* ret = abi::__cxa_demangle(mangledName, NULL, &length, &status);

    if(status == 0)
    {
        //demangling succeeded
        std::string retStr(ret, length);
        free(ret);

        return retStr;
    }
    else
    {
        //there was an error
        //construct an error message and throw an exception
        std::ostringstream os;
        os << "Error in abi::__cxa_demangle while demangling "
            << mangledName << ": ";
        switch(status)
        {
            case -1:
                os << "a memory allocation failure occurred.";
                break;
            case -2:
                os << mangledName << " is not a valid name under "
                    << "the C++ ABI mangling rules.";
                break;
            case -3:
                os << "one of the arguments is invalid.";
                break;
            default:
                os << "reason unknown.";
                break;
        }

        os << std::endl;
        throw DemanglingException(os.str());
    }
}

#else
//demangling is a no-op
std::string demangle(const char* mangledName) {
    return std::string(mangledName);
}
#endif
}


template <typename L, typename R>
class Either
{
public:
    enum Type
    {
        LEFT,
        RIGHT
    };

    const char* typeToString(Type t)
    {
        if(t == LEFT)
        {
            return "LEFT";
        }
        else
        {
            return "RIGHT";
        }
    }

    //exception type for invalid access
    class EitherException : public std::runtime_error
    {
    public:
        EitherException(const std::string& what_arg)
            : std::runtime_error(what_arg)
        {}
    };
    
    std::string getLTypeName()
    {
        return demangle(typeid(L).name());
    }

    std::string getRTypeName()
    {
        return demangle(typeid(R).name());
    }

private:
    std::unique_ptr<L> lPtr;
    std::unique_ptr<R> rPtr;

    Type type;


    void throwException(Type triedType)
    {
        std::ostringstream os;
        os << "Exception in Either[" << getLTypeName() << ", "
            << getRTypeName() << "]: "
            << "Tried to access type of " << typeToString(triedType)
            << " but this Either object is a " << typeToString(type)
            << std::endl;
        throw EitherException(os.str());
    }
public:
    Either(std::unique_ptr<L> lPtr, 
            std::unique_ptr<R> rPtr, 
            Type type)
        : lPtr(lPtr), rPtr(rPtr), type(type)
    {}

    Either(std::unique_ptr<L> lPtr)
        : Either(lPtr, nullptr, LEFT)
    {}

    Either(std::unique_ptr<R> rPtr)
        : Either(nullptr, rPtr, RIGHT)
    {}

    //get*Ptr() methods return nullptr
    //on incorrect type
    std::unique_ptr<L> getLeftPtr()
    {
        if(type == RIGHT)
        {
            return nullptr;
        }
        else
        {
            return lPtr;
        }
    }

    std::unique_ptr<R> getRightPtr()
    {
        if(type == LEFT)
        {
            return nullptr;
        }
        else
        {
            return rPtr;
        }
    }

    //getLeft and getRight throw an exception on incorrect type
    L& getLeft()
    {
        if(type == RIGHT)
        {
            throwException();
        }
        else
        {
            return *lPtr;
        }
    }

    R& getRight()
    {
        if(type == LEFT)
        {
            throwException();
        }
        else
        {
            return *rPtr;
        }
    }


    Type getType()
    {
        return type;
    }
};
