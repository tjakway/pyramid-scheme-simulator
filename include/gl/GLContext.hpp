#pragma once


#include "NamespaceDefines.hpp"
#include "gl/GLWindow.hpp"

#include <string>
#include <memory>

BEGIN_PYRAMID_GL_NAMESPACE

class GLContext
{
protected:
    static const std::string vertexShaderSource,
                             fragmentShaderSource;

    static const std::string windowTitle,
                 applicationId;
    
    std::unique_ptr<GLWindow> glWindow;
    
    //rendering callbacks
    void glInit();
    void glDraw();
    void glCleanup();

public:

    GLContext();

    void run();
    void run(Glib::RefPtr<Application>);
};

END_PYRAMID_GL_NAMESPACE
