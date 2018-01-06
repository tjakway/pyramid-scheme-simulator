#pragma once

#include "NamespaceDefines.hpp"
#include "Util/Util.hpp"
#include "Util/NewExceptionType.hpp"

#include "gl/GLResourceHandle.hpp"
#include "gl/GLUtil.hpp"
#include "gl/ShaderDefines.hpp"

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
                     numColorDimensions = 4,    //r, g, b, a
                     
                     //AKA stride
                     totalOffset = sizeof(GLfloat) *  //stride needs to be in bytes
                                    (numPositionDimensions + 
                                     numTexCoordDimensions + 
                                     numColorDimensions),
                     
                     //the start of each dimension's data in the array
                     positionOffset = sizeof(GLfloat) * 0,
                     texCoordOffset = positionOffset + (sizeof(GLfloat) * numPositionDimensions),
                     colorOffset = texCoordOffset + (sizeof(GLfloat) * numTexCoordDimensions);


    virtual std::array<float, numPositionDimensions*numVertices> getPositionData();
    virtual std::array<float, numTexCoordDimensions*numVertices> getTexCoordData();
    virtual std::array<float, numColorDimensions*numVertices> getColorData();

    static constexpr int numVBOElements = 
        (numPositionDimensions + numTexCoordDimensions + numColorDimensions) * numVertices;

    //combine the arrays of each dimension
    std::array<float, numVBOElements> interleaveVertexData();

    InternalVAOHandle genVAO(GLenum hint = GL_STATIC_DRAW)
    {
        InternalVAOHandle handle;

        //generate ids
        glGenVertexArrays(1, &handle.vaoId);
        glBindVertexArray(handle.vaoId);

        glGenBuffers(1, &handle.vboId);

        //get the vertex data
        const auto vertexData = interleaveVertexData();

        //fill the vbo
        glBindBuffer(GL_ARRAY_BUFFER, handle.vboId);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * numVBOElements, vertexData.data(), hint);
        
        //indicate data offsets for each attribute
        glVertexAttribPointer(VS_POSITION_LOC, )
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
