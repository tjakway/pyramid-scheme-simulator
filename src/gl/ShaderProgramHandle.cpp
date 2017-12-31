#include "gl/ShaderHandle.hpp"

BEGIN_PYRAMID_GL_NAMESPACE


ShaderProgramHandle::ShaderProgramHandle(GLuint handle)
    : GLResourceHandle(handle)
{}

ShaderProgramHandle::ShaderProgramHandle(ShaderProgramHandle&& handle)
    : GLResourceHandle(std::move(handle))
{}

GLuint ShaderProgramHandle::get() const
{
    GLUtil::throwIfErrorInShader(handle);
    return handle;
}

ShaderProgramHandle ShaderProgramHandle::loadShaderProgramFromStrings(
        const std::string& vertexShaderSrc,
        const std::string& fragmentShaderSrc)
{
    return ShaderProgramHandle(
            GLUtil::compileShaderProgram(vertexShaderSrc, fragmentShaderSrc));
}

END_PYRAMID_GL_NAMESPACE
