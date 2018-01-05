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


ShaderProgramHandle::ShaderProgramHandle()
    : GLResourceHandle()
{}

void ShaderProgramHandle::freeResource(GLuint programHandle)
{
    if(glIsProgram(programHandle) == GL_TRUE)
    {
        //detach & delete the individual shaders
        GLint numShaders = -1;
        glGetProgramiv(programHandle, GL_ATTACHED_SHADERS, &numShaders);
        GLUtil::throwIfError();

        GLsizei actualNumShaders = -1;
        if(numShaders > 0)
        {
            GLuint attachedShaders[numShaders];
            glGetAttachedShaders(programHandle, numShaders, &actualNumShaders, attachedShaders);
            GLUtil::throwIfError();

            if(numShaders != actualNumShaders)
            {
                throw GLUtil::OpenGLException(STRCAT("numShaders != actualNumShaders in ", __func__,
                            ", numShaders (expected): ", numShaders, 
                            ", actualNumShaders: ", actualNumShaders));
            }

            for(GLint i = 0; i < numShaders; i++)
            {
                glDetachShader(programHandle, attachedShaders[i]);
                glDeleteShader(attachedShaders[i]);

                GLUtil::throwIfError();
            }
        }


        glDeleteProgram(programHandle);
    }
}

ShaderProgramHandle ShaderProgramHandle::loadShaderProgramFromStrings(
        const std::string& vertexShaderSrc,
        const std::string& fragmentShaderSrc)
{
    return ShaderProgramHandle(
            GLUtil::compileShaderProgram(vertexShaderSrc, fragmentShaderSrc));
}

END_PYRAMID_GL_NAMESPACE
