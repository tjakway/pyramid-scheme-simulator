#include "gl/GLUtil.hpp"

#include "Util/Strcat.hpp"

#include <memory>

#include <GL/gl.h>


void GLUtil::throwIfErrorInShader(GLuint shader)
{
    GLint status = -1;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    if(status != GL_TRUE)
    {
        throw GLUtilException(STRCAT("Error in shader "));
    }
}

/**
 * returns the info log for that shader or a blank string if there isn't one
 */
std::string GLUtil::getShaderInfoLog(GLuint shader, )
{
    GLint logLength = -1;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);

    if(logLength > 0)
    {
        std::unique_ptr<char> shaderInfoLog 
            = std::unique_ptr<char>(new char[logLength]);

        GLint writtenLength = -1;
        glGetShaderInfoLog(shader, logLength, &writtenLength, shaderInfoLog.get());

        std::string infoLog(shaderInfoLog.get());

        if(writtenLength != logLength)
        {
            throw GLUtil::GLUtilException(STRCAT(
                    "Got log length of ", writtenLength, " but expected ", logLength, " for "
                    " info log: ", infoLog));
        }
        else
        {
            return infoLog;
        }
    }
    else
    {
        return std::string();
    }
}

GLuint GLUtil::compileShaderProgram(
        std::string vertexShaderSrc, 
        std::string fragmentShaderSrc)
{
    const GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    if(vs == 0)
    {

    }

}
