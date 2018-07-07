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
        return surroundPrettyPrint(prettyPrintImpl());
    }

    template <typename T> static std::string getClassNameOrNull(T* ptr)
    {
        std::string name;
        if(ptr == nullptr)
        {
            name = "nullptr of type " + std::string(typeid(ptr).name());
        }
        else
        {
            name = typeid(ptr).name();
        }

        return surroundName(name);
    }

    /**
     * so you can pretty print pointers without checking if they're null
     */
    template <typename T> static std::string tryPrettyPrint(T* ptr)
    {
        if(ptr == nullptr)
        {
            return surroundPrettyPrint(getClassNameOrNull(ptr));
        }
        else
        {
            return ptr->prettyPrint();
        }
    }
};
