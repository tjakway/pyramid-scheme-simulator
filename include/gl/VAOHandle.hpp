#pragma once

#include "NamespaceDefines.hpp"
#include "Util/Util.hpp"
#include "Util/NewExceptionType.hpp"

#include "gl/GLResourceHandle.hpp"
#include "gl/GLUtil.hpp"
#include "gl/ShaderDefines.hpp"

#include <GL/glew.h>

#include <functional>
#include <utility>
#include <array>
#include <vector>

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
                     
                     stride = sizeof(GLfloat) *  //stride needs to be in bytes
                                    (numPositionDimensions + 
                                     numTexCoordDimensions + 
                                     numColorDimensions),
                     
                     //the start of each dimension's data in the array
                     positionOffset = sizeof(GLfloat) * 0,
                     texCoordOffset = positionOffset + (sizeof(GLfloat) * numPositionDimensions),
                     colorOffset = texCoordOffset + (sizeof(GLfloat) * numTexCoordDimensions),

                     numPositionElems = numPositionDimensions*numVertices,
                     numTexCoordElems = numTexCoordDimensions*numVertices,
                     numColorElems    = numColorDimensions*numVertices;

    virtual std::array<float, numPositionElems> getPositionData() const;
    virtual std::array<float, numTexCoordElems> getTexCoordData() const;
    virtual std::array<float, numColorElems> getColorData() const;

    static constexpr int numVBOElements = 
        (numPositionDimensions + numTexCoordDimensions + numColorDimensions) * numVertices;

    /** combine the arrays of each dimension
     * it's definitely possible to do this as a constexpr and return an std::array
     * but C++11 does *not* make it easy
     *
     * returned vector size should be equal to numVBOElements
     */
    std::vector<float> interleaveVertexData();

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
        GLUtil::throwIfError();


        //indicate offsets into the VBO for each attribute
        //position data:
        glVertexAttribPointer(VS_POSITION_LOC, 
                numPositionDimensions,
                GL_FLOAT, 
                GL_FALSE,
                //stride is the same for all of these calls because we're using the same VBO
                stride,
                //the last argument is an int-ish value that's added the VBO pointer
                (void*)(sizeof(GLfloat) * positionOffset)); 
        //enable the attribute for this VAO
        glEnableVertexAttribArray(VS_POSITION_LOC);
        GLUtil::throwIfError();

        //tex coord data:
        glVertexAttribPointer(VS_TEXCOORD_LOC, 
                numTexCoordDimensions,
                GL_FLOAT, 
                GL_FALSE,
                stride,
                (void*)(sizeof(GLfloat) * texCoordOffset)); 
        glEnableVertexAttribArray(VS_TEXCOORD_LOC);
        GLUtil::throwIfError();

        //color data:
        glVertexAttribPointer(VS_COLOR_LOC, 
                numColorDimensions,
                GL_FLOAT, 
                GL_FALSE,
                stride,
                (void*)(sizeof(GLfloat) * colorOffset)); 
        glEnableVertexAttribArray(VS_COLOR_LOC);
        GLUtil::throwIfError();


        return handle;
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
    VAOHandle()
        : GLResourceHandle(genVAO())
    {
        setErrorChecker(std::bind(&VAOHandle::errorCheck, std::placeholders::_1));
    }

    VAOHandle(VAOHandle&& other)
        : GLResourceHandle(std::move(other))
    {}

    void bind() { glBindVertexArray(get().vaoId); }

    void draw();



    //the color you get if you don't override getColorData()
    static const std::array<float, numColorDimensions> defaultSolidColor;
};

END_PYRAMID_GL_NAMESPACE
