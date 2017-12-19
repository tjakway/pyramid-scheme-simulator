#pragma once

#include <memory>
#include <stdexcept>
#include <string>
#include <sstream>
#include <utility>
#include <typeinfo>
#include <cstdlib>
#include <cstring>
#include <cctype>

#ifndef EITHER_HPP_DEMANGLE_NAMES

/* Test for GCC > 3.0.0  and libstdc++ */
#if __GNUC__ > 3 && defined __GLIBCPP__
#define EITHER_HPP_DEMANGLE_NAMES
#endif

#endif

namespace {
#if defined EITHER_HPP_DEMANGLE_NAMES && !(defined NO_EITHER_HPP_DEMANGLE_NAMES)
//implement name de-mangling

//see https://gcc.gnu.org/onlinedocs/libstdc++/libstdc++-html-USERS-4.3/a01696.html
#include <cxxabi.h>

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

    char* ret = __cxxabiv1::__cxa_demangle(mangledName, NULL, &length, &status);

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
//don't warn about emitting an unused function
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunneeded-internal-declaration"
std::string demangle(const char* mangledName) {
    return std::string(mangledName);
}
#pragma GCC diagnostic pop
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

    const char* typeToString(Type t) const
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

    class EitherTypeException : public EitherException
    {
    public:
        EitherTypeException(const std::string& what_arg)
            : EitherException(what_arg)
        {}
    };
    
    class EitherNullException : public EitherException
    {
    public:
        EitherNullException(const std::string& what_arg)
            : EitherException(what_arg)
        {}
    };

    std::string getLTypeName() const
    {
        return demangle(typeid(L).name());
    }

    std::string getRTypeName() const
    {
        return demangle(typeid(R).name());
    }

private:
    std::unique_ptr<L> lPtr;
    std::unique_ptr<R> rPtr;

    Type type;

    bool underlyingValueIsNull() const
    {
        if(type == LEFT && lPtr == nullptr)
        {
            return true;
        }
        else if(type == RIGHT && rPtr == nullptr)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    std::string getExceptionHeader() const
    {
        std::ostringstream os;
        os << "Exception in Either[" << 
            getLTypeName() << ", "
            << getRTypeName() << "]: ";
        return os.str();
    }

    [[noreturn]] void throwTypeException(Type triedType) const
    {
        std::ostringstream os;
        os << getExceptionHeader()
            << "Tried to access type of " << typeToString(triedType)
            << " but this Either object is a " << typeToString(type)
            << std::endl;
        throw EitherTypeException(os.str());
    }

    [[noreturn]] void throwValueException() const
    {
        std::ostringstream os;
        os << getExceptionHeader()
            << "Have type " << typeToString(type)
            << " but the underlying value is null"
            << std::endl;
        throw EitherTypeException(os.str());
    }
public:
    Either(std::unique_ptr<L>&& lPtr, 
            std::unique_ptr<R>&& rPtr, 
            Type type)
        : lPtr(std::move(lPtr)), rPtr(std::move(rPtr)), type(type)
    {}

    Either(std::unique_ptr<L>&& lPtr)
        : Either(std::move(lPtr), nullptr, LEFT)
    {}

    Either(std::unique_ptr<R>&& rPtr)
        : Either(nullptr, std::move(rPtr), RIGHT)
    {}


    Either(Either&& other)
        : Either(std::move(other.lPtr), std::move(other.rPtr), other.type)
    {}

    static Either<L, R> left(std::unique_ptr<L>&& lPtr)
    {
        return Either<L, R>(std::move(lPtr), nullptr, LEFT);
    }


    static Either<L, R> right(std::unique_ptr<R>&& rPtr)
    {
        return Either<L, R>(nullptr, std::move(rPtr), RIGHT);
    }

    //get*Ptr() methods return nullptr
    //on incorrect type
    std::unique_ptr<L> getLeftPtr() const
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

    std::unique_ptr<R> getRightPtr() const
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
    L& getLeft() const
    {
        if(type == RIGHT)
        {
            throwTypeException(RIGHT);
        }
        else
        {
            if(underlyingValueIsNull())
            {
                throwValueException();
            }
            else
            {
                return *lPtr;
            }
        }
    }

    R& getRight() const
    {
        if(type == LEFT)
        {
            throwTypeException(RIGHT);
        }
        else
        {
            if(underlyingValueIsNull())
            {
                throwValueException();
            }
            else
            {
                return *rPtr;
            }
        }
    }


    Type getType() const
    {
        return type;
    }

    bool isLeft() const
    {
        return type == LEFT;
    }

    bool isRight() const
    {
        return type == RIGHT;
    }

    bool operator==(const Either<L, R>& other) const
    {
        //compare types
        if(type != other.type)
        {
            return false;
        }
        //if the types match compare the underlying objects
        else if(isLeft())
        {
            return getLeft() == other.getLeft();
        }
        else if(isRight())
        {
            return getRight() == other.getRight();
        }
        //sanity check
        else
        {
            throw "Unrecognized type.";
        }
    }
};
