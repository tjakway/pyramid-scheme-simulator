#pragma once

#include "NamespaceDefines.hpp"

#include <string>
#include <GL/gl.h>

BEGIN_PYRAMID_GL_NAMESPACE

class TextureHandle
{
protected:
    const GLuint handle;
    TextureHandle(GLuint);

public:
    TextureHandle(TextureHandle&&);
    virtual ~TextureHandle();

    //TODO: if debug, check texture handle
    GLuint get() const;

    static TextureHandle loadTextureFromFile(std::string);
};

END_PYRAMID_GL_NAMESPACE
