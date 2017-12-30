#include "gl/GLUtil.hpp"

#include "Util/Strcat.hpp"

#include <functional>
#include <memory>
#include <sstream>

#include <GL/glew.h>
#include <GL/gl.h>


void GLUtil::throwIfError()
{
#ifndef GLUTIL_SKIP_THOROUGH_ERROR_CHECKS
    bool errorFound = false;
    std::ostringstream errStream;

    GLenum errorFlag = glGetError();
    while(errorFlag != GL_NO_ERROR)
    {
        errStream << "OpenGL error found: " << '\t' << 
            gluErrorString(errorFlag) << std::endl;
        errorFound = true;
    }

    if(errorFound)
    {
        throw OpenGLException(errStream.str());
    }
#elif defined(GLUTIL_SKIP_THOROUGH_ERROR_CHECKS) && defined(DEBUG)
#warning "Skipping thorough warning checks in GLUtil::throwIfError in a debug build" 
#endif


}

std::string GLUtil::getShaderInfoLog(GLuint shader)
{
    return GLUtil::getShaderStringValue(shader, GL_INFO_LOG_LENGTH, 
            [](GLuint s, GLsizei m, GLsizei* l, GLchar* i) {
                glGetShaderInfoLog(s, m, l, i);
            });
}

std::string GLUtil::getShaderSource(GLuint shader)
{
    return GLUtil::getShaderStringValue(shader, GL_SHADER_SOURCE_LENGTH, 
            [](GLuint s, GLsizei m, GLsizei* l, GLchar* i) {
                glGetShaderSource(s, m, l, i);
            });
}

void GLUtil::throwIfErrorInShader(GLuint shader)
{
    GLint status = -1;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    if(status != GL_TRUE)
    {
        throw GLUtilException(STRCAT("Error in shader with source: ",
                    std::endl, getShaderSource(shader),
                    std::endl, "shader info log: ",
                    getShaderInfoLog(shader), std::endl));
    }
}

/**
 * returns the info log for that shader or a blank string if there isn't one
 */
std::string GLUtil::getShaderStringValue(
        GLuint shader, 
        GLenum lengthEnum, 
        std::function<void(GLuint, GLsizei, GLsizei*, GLchar*)> query)
{
    GLint lengthParam = -1;
    glGetShaderiv(shader, lengthEnum, &lengthParam);

    if(lengthParam > 0)
    {
        std::unique_ptr<char> buf 
            = std::unique_ptr<char>(new char[lengthParam]);

        GLint writtenLength = -1;
        query(shader, lengthParam, &writtenLength, buf.get());

        std::string strValue(buf.get());

        if(writtenLength != lengthParam)
        {
            throw GLUtil::GLUtilException(STRCAT(
                    "Got length of ", writtenLength, " but expected ", lengthParam, " for "
                    " shader string characteristic: ", strValue));
        }
        else
        {
            return strValue;
        }
    }
    else
    {
        return std::string();
    }
}

void GLUtil::assertIsTexture(GLuint handle)
{
#ifdef DEBUG
    if(glIsTexture(handle) != GL_TRUE)
    {
        throw OpenGLException(STRCAT("Handle ", handle,
                    " does not refer to a texture."));
    }
#endif
}

GLuint GLUtil::compileShaderProgram(
        std::string vertexShaderSrc, 
        std::string fragmentShaderSrc)
{
    const GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    if(vs == 0)
    {

    }

    return vs;
}
