#include "gl/TextureHandle.hpp"
#include "gl/GLUtil.hpp"
#include "NamespaceDefines.hpp"

BEGIN_PYRAMID_GL_NAMESPACE


TextureHandle::TextureHandle(GLuint _handle)
    : handle(_handle)
{}

TextureHandle::TextureHandle(TextureHandle&& other)
    : handle(other.handle)
{}

TextureHandle::~TextureHandle()
{
    glDeleteTextures(1, &handle);
}

GLuint TextureHandle::get() const
{
    GLUtil::assertIsTexture(handle);
    return handle;
}

END_PYRAMID_GL_NAMESPACE
