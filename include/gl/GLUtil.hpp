#pragma once

#include <GL/gl.h>

#include "Util/NewExceptionType.hpp"

class GLUtil
{
    GLUtil() = delete;

    static std::string getShaderInfoLog(GLuint);
    static void throwIfErrorInShader(GLuint);

public:
    NEW_EXCEPTION_TYPE(GLUtilException);

    static GLuint compileShaderProgram(std::string vertexShader, 
            std::string fragmentShader);
};
