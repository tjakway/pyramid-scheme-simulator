#include "gl/TextureHandle.hpp"
#include "gl/GLUtil.hpp"
#include "gl/ImageLoader.hpp"
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


TextureHandle TextureHandle::loadTextureFromPNG(const std::string& filename)
{
    return TextureHandle::loadTextureFromPNG(filename.c_str());
}

TextureHandle TextureHandle::loadTextureFromPNG(const char* filename)
{
    //TODO: error if texture dimensions are non-power of 2?

    const ImageLoader::Image imageData = ImageLoader::loadPNG(filename);
    
    GLuint handle;
    glGenTextures(1, &handle);

    glBindTexture(GL_TEXTURE_2D, handle);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageData.width,
            imageData.height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
            imageData.pixels.data());

    GLUtil::assertIsTexture(handle);

    return TextureHandle(handle);
}

END_PYRAMID_GL_NAMESPACE
