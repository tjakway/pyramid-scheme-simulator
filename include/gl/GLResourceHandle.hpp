#pragma once

#include "NamespaceDefines.hpp"
#include "Util/Util.hpp"

#include <memory>

BEGIN_PYRAMID_GL_NAMESPACE

/**
 * encapsulates some OpenGL resource handle of type T
 * (e.g. GLuint)
 */
template <typename T>
class GLResourceHandle
{
protected:
    std::unique_ptr<T, std::function<void(T*)>> handlePtr;

    GLResourceHandle(T _handle)
        : handlePtr(new T(_handle), deleterObject)
    {}

    const std::function<void(T*)> deleterObject = 
        //dereference the pointer so subclasses don't have to bother with it
        [this](T* toDelete){
            this->freeResource(*toDelete);
            delete toDelete;
        };

    std::function<void(T)> errorCheck = [](T){};

    //set an error checking function to be called in get()
    void setErrorChecker(std::function<void(T)> f)
    {
        errorCheck = f;
    }

    void callErrorChecker(T handle)
    {
        errorCheck(handle);
    }
    
    //subclasses just have to implement this to get the right cleanup semantics
    virtual void freeResource(T resourceHandle) = 0;

public:
    GLResourceHandle(GLResourceHandle&& other)
        : handlePtr(std::move(other.handlePtr))
    {}

    virtual GLResourceHandle<T>& operator=(GLResourceHandle<T>&& other)
    {
        if(this != &other)
        {
            handlePtr = std::move(other.handlePtr);
        }

        return *this;
    }

    virtual ~GLResourceHandle() {}
    virtual T get() const
    {
        errorCheck(*handlePtr);
        return *handlePtr;
    }
};


END_PYRAMID_GL_NAMESPACE
