#pragma once

#include "NamespaceDefines.hpp"

BEGIN_PYRAMID_GL_NAMESPACE

/**
 * encapsulates some OpenGL resource handle of type T
 * (e.g. GLuint)
 */
template <typename T>
class GLResourceHandle
{
protected:
    const T handle;
    GLResourceHandle(T _handle)
        : handle(_handle)
    {}

public:
    GLResourceHandle(GLResourceHandle&& other)
        : handle(other.handle)
    {}

    virtual ~GLResourceHandle() {}
    virtual GLuint get() const = 0;
};


BEGIN_PYRAMID_GL_NAMESPACE
