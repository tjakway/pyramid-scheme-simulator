#pragma once

#include "NamespaceDefines.hpp"

#include "gl/GLResourceHandle.hpp"
#include "gl/GLUtil.hpp"

#include <string>
#include <GL/glew.h>

BEGIN_PYRAMID_GL_NAMESPACE

class ShaderProgramHandle : public GLResourceHandle<GLuint, ShaderProgramHandle>
{
protected:
    ShaderProgramHandle(GLuint);

    virtual void freeResource(GLuint) override;

public:
    ShaderProgramHandle(ShaderProgramHandle&&);
    ShaderProgramHandle();

    virtual ~ShaderProgramHandle() {}


    //needed because of https://stackoverflow.com/questions/15351341/move-constructors-and-inheritance
    virtual ShaderProgramHandle& operator=(ShaderProgramHandle&& o) override
    { return GLResourceHandle::operator=(std::move(o)); }

    static ShaderProgramHandle loadShaderProgramFromStrings(
            const std::string&, const std::string&);
};


END_PYRAMID_GL_NAMESPACE
