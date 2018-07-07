#pragma once

#include <typeinfo>
#include <string>

#include <memory>

class PrettyPrintable
{
     //standardize pretty print formatting

    static std::string surroundPrettyPrint(std::string str)
    {
        return "[" + str + "]";
    }

    static std::string surroundName(std::string str)
    {
        return "<" + str + ">";
    }

    /**
     * derived classes can only implement prettyPrintImpl so we can control formatting
     * of the final message from here
     */
    virtual std::string prettyPrintImpl() const = 0;

public:
    std::string prettyPrint() const
    {
        surroundPrettyPrint(prettyPrintImpl());
    }

    template <typename T> static std::string getClassNameOrNull(T* ptr)
    {
        std::string name;
        if(ptr == nullptr)
        {
            name = "nullptr of type " + typeinfo(ptr).name();
        }
        else
        {
            name = typeinfo(ptr).name();
        }

        return surroundName(name);
    }


    /**
     * specializations for unique_ptr and shared_ptr
     */
    template <typename X> static std::string getClassNameOrNull(std::unique_ptr<X> ptr)
    {
        return getClassNameOrNull(ptr.get());
    }

    template <typename X> static std::string getClassNameOrNull(std::shared_ptr<X> ptr)
    {
        return getClassNameOrNull(ptr.get());
    }
};
