#pragma once

#include "NamespaceDefines.hpp"
#include "gl/GLResourceHandle.hpp"

#include <string>
#include <GL/glew.h>

BEGIN_PYRAMID_GL_NAMESPACE

class TextureHandle : public GLResourceHandle<GLuint, TextureHandle>
{
protected:
    TextureHandle(GLuint);

public:
    TextureHandle(TextureHandle&&);
    virtual ~TextureHandle();

    virtual void freeResource(GLuint) override;

    static TextureHandle loadTextureFromPNG(const char*);
    static TextureHandle loadTextureFromPNG(const std::string&);
};

END_PYRAMID_GL_NAMESPACE
