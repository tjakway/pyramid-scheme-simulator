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
        : handlePtr(make_unique<T>(_handle), deleterObject)
    {}

    const std::function<void(T*)> deleterObject = 
        std::bind(&GLResourceHandle::freeResource, this, std::placeholders::_1);

    //subclasses just have to implement this to get the right cleanup semantics
    virtual void freeResource(T* resource) = 0;

public:
    GLResourceHandle(GLResourceHandle&& other)
        : handlePtr(std::move(other.handlePtr), deleterObject)
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
        return *handlePtr;
    }
};


END_PYRAMID_GL_NAMESPACE
