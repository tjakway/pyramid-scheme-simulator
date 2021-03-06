#pragma once

#include "NamespaceDefines.hpp"
#include "Util/Util.hpp"

#include <memory>

BEGIN_PYRAMID_GL_NAMESPACE

/**
 * encapsulates some OpenGL resource handle of type T
 * (e.g. GLuint)
 */
template <typename T, typename XType>
class GLResourceHandle
{
protected:
    std::unique_ptr<T, std::function<void(T*)>> handlePtr;

    GLResourceHandle(T _handle)
        : handlePtr(new T(_handle), getDeleterObject())
    {}

    GLResourceHandle()
    {}

    std::function<void(T*)> getDeleterObject() {
        //dereference the pointer so subclasses don't have to bother with it
        [this](T* toDelete){
            this->freeResource(*toDelete);
            delete toDelete;
        };
    }

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

    virtual XType& operator=(XType&& other)
    {
        if(this != &other)
        {
            handlePtr = std::move(other.handlePtr);
        }

        return static_cast<XType&>(*this);
    }

    //don't need to call freeResource because unique_ptr will call it for us
    virtual ~GLResourceHandle() {}

    virtual T get() const
    {
        errorCheck(*handlePtr);
        return *handlePtr;
    }
};


END_PYRAMID_GL_NAMESPACE
