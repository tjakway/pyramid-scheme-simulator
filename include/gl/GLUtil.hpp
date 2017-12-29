#pragma once

#include <GL/glew.h>
#include <GL/gl.h>

#include <string>
#include <functional>

#include "Util/NewExceptionType.hpp"

class GLUtil
{
    GLUtil() = delete;

    static void throwIfErrorInShader(GLuint);

    
    static std::string getShaderStringValue(
        GLuint, 
        GLenum, 
        std::function<void(GLuint, GLsizei, GLsizei*, GLchar*)>);

    static std::string getShaderInfoLog(GLuint);
    static std::string getShaderSource(GLuint);

public:
    NEW_EXCEPTION_TYPE(OpenGLException);
    NEW_EXCEPTION_TYPE_WITH_BASE(GLUtilException, OpenGLException);

    static void throwIfError();

    static GLuint compileShaderProgram(std::string vertexShader, 
            std::string fragmentShader);
};
