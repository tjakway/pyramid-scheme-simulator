#include "gl/ShaderProgramHandle.hpp"

BEGIN_PYRAMID_GL_NAMESPACE


ShaderProgramHandle::ShaderProgramHandle(GLuint handle)
    : GLResourceHandle(handle)
{
    setErrorChecker(std::bind(&GLUtil::throwIfErrorInShader, std::placeholders::_1));
}

ShaderProgramHandle::ShaderProgramHandle(ShaderProgramHandle&& handle)
    : GLResourceHandle(std::move(handle))
{}


void ShaderProgramHandle::freeResource(GLuint handle)
{
    callErrorChecker(handle);

}

ShaderProgramHandle ShaderProgramHandle::loadShaderProgramFromStrings(
        const std::string& vertexShaderSrc,
        const std::string& fragmentShaderSrc)
{
    return ShaderProgramHandle(
            GLUtil::compileShaderProgram(vertexShaderSrc, fragmentShaderSrc));
}

END_PYRAMID_GL_NAMESPACE
