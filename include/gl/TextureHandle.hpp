#pragma once

#include "NamespaceDefines.hpp"
#include "gl/GLResourceHandle.hpp"

#include <string>
#include <GL/glew.h>

BEGIN_PYRAMID_GL_NAMESPACE

class TextureHandle : public GLResourceHandle<GLuint>
{
protected:
    TextureHandle(GLuint);

public:
    TextureHandle(TextureHandle&&);
    virtual ~TextureHandle();

    virtual GLuint get() const override;

    static TextureHandle loadTextureFromPNG(std::string);
};

END_PYRAMID_GL_NAMESPACE
