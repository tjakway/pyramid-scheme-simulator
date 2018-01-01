#pragma once

#include "NamespaceDefines.hpp"

#include "gl/GLResourceHandle.hpp"
#include "gl/GLUtil.hpp"

#include <string>
#include <GL/glew.h>

BEGIN_PYRAMID_GL_NAMESPACE

class ShaderProgramHandle : public GLResourceHandle<GLuint>
{
protected:
    ShaderProgramHandle(GLuint);

    virtual void freeResource(GLuint) override;

public:
    ShaderProgramHandle(ShaderProgramHandle&&);
    virtual ~ShaderProgramHandle();

    static ShaderProgramHandle loadShaderProgramFromStrings(
            const std::string&, const std::string&);
};


END_PYRAMID_GL_NAMESPACE
