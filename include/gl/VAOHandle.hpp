#pragma once

#include "NamespaceDefines.hpp"
#include "Util/Util.hpp"
#include "Util/NewExceptionType.hpp"

#include "gl/GLResourceHandle.hpp"
#include "gl/GLUtil.hpp"

#include <GL/glew.h>

#include <utility>
#include <array>

BEGIN_PYRAMID_GL_NAMESPACE

class VAOHandle;

//more informative replacement for std::pair
class InternalVAOHandle
{
    friend VAOHandle;
private:
    GLuint vaoId, vboId;

public:
    GLuint get() { return vaoId; }
};

/**
 * a VAO needs to store a VBO in addition to the VAO itself,
 * hence the InternalVAOHandle
 *
 * the downside is you have to call get() twice, once on the GLResourceHandle and again on
 * the InternalVAOHandle
 */
class VAOHandle : public GLResourceHandle<InternalVAOHandle, VAOHandle>
{
public:
    NEW_EXCEPTION_TYPE_WITH_BASE(VAOException, GLUtil::OpenGLException);

private:
    //triangle strip requires 4 vertices to draw a square
    static constexpr int numVertices = 4,
                     numPositionDimensions = 3, //x, y, z
                     numTexCoordDimensions = 2, //u, v
                     numColorDimensions = 4;    //r, g, b, a

    virtual std::array<float, numPositionDimensions*numVertices> getPositionData();
    virtual std::array<float, numTexCoordDimensions*numVertices> getTexCoordData();
    virtual std::array<float, numColorDimensions*numVertices> getColorData();


    static InternalVAOHandle genVAO()
    {
        InternalVAOHandle handle;

        //generate ids
        glGenVertexArrays(1, &handle.vaoId);
        glBindVertexArray(handle.vaoId);

        glGenBuffers(1, &handle.vboId);

        //fill the vbo
        
    }

    static void errorCheck(InternalVAOHandle handle)
    {
        if(glIsVertexArray(handle.vaoId) != GL_TRUE)
        {
            throw VAOException("vaoId does not refer to a valid VAO");
        }

        if(glIsBuffer(handle.vboId) != GL_TRUE)
        {
            throw VAOException("vboId does not refer to a valid VBO");
        }
    }


    virtual void freeResource(InternalVAOHandle handle) override
    {
        if(glIsVertexArray(handle.vaoId))
        {
            glDeleteBuffers(1, &handle.vaoId);
        }

        if(glIsBuffer(handle.vboId))
        {
            glDeleteBuffers(1, &handle.vboId);
        }
    }

public:


};

END_PYRAMID_GL_NAMESPACE
