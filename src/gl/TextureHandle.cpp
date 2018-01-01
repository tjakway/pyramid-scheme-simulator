#include "gl/TextureHandle.hpp"
#include "gl/GLUtil.hpp"
#include "gl/ImageLoader.hpp"
#include "NamespaceDefines.hpp"

#include <utility>

BEGIN_PYRAMID_GL_NAMESPACE


TextureHandle::TextureHandle(GLuint _handle)
    : GLResourceHandle(_handle)
{
    setErrorChecker(std::bind(&GLUtil::assertIsTexture, std::placeholders::_1));
}

TextureHandle::TextureHandle(TextureHandle&& other)
    : GLResourceHandle(std::move(other))
{
    setErrorChecker(std::bind(&GLUtil::assertIsTexture, std::placeholders::_1));
}

TextureHandle::~TextureHandle()
{
    const GLuint handle = get();
    glDeleteTextures(1, &handle);
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
