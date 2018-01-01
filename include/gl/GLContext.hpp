#pragma once


#include "Config.hpp"
#include "NamespaceDefines.hpp"
#include "gl/GLWindow.hpp"
#include "gl/GLResourceHandle.hpp"
#include "gl/ShaderProgramHandle.hpp"

#include <string>
#include <memory>

BEGIN_PYRAMID_GL_NAMESPACE

class GLContext
{
protected:
    static const std::string vertexShaderSource,
                             fragmentShaderSource;

    static const int openglRequiredMajorVersion,
                     openglRequiredMinorVersion;
    
    static const std::string windowTitle,
                 applicationId;
    
    std::unique_ptr<GLWindow> glWindow;

    //opengl data
    ShaderProgramHandle shaderProgramHandle;
    
    //rendering callbacks
    void glInit();
    void glDraw();
    void glCleanup();

public:

    GLContext(const Config::BackendOptions::GLBackendOptions::WindowOptions&);

    void run();
    void run(Glib::RefPtr<Gtk::Application>);
};

END_PYRAMID_GL_NAMESPACE
