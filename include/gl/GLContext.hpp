#pragma once


#include "Config.hpp"
#include "NamespaceDefines.hpp"
#include "gl/GLWindow.hpp"

#include "gl/MainRenderer.hpp"


#include <string>
#include <memory>

BEGIN_PYRAMID_GL_NAMESPACE

class GLContext
{
protected:
    static const int openglRequiredMajorVersion,
                     openglRequiredMinorVersion;
    
    static const std::string windowTitle,
                 applicationId;
    
    std::unique_ptr<GLWindow> glWindow;

    std::unique_ptr<MainRenderer> mainRenderer;

    //rendering callbacks
    void glInit();
    void glDraw();
    void glCleanup();

public:
    GLContext(const Config::BackendOptions::GLBackendOptions::WindowOptions&,
            std::pair<std::unique_ptr<GraphLayout::Graph>, GraphLayout::BoundingBox>);

    void run();

    //pass an instance of std::chrono::duration
    //GLContext will not poll for events longer than the passed timeout
    //BUT more time may elapse in the draw loop
    //
    //i.e. runWithTimeout is useful to stop GLContext from spinning while waiting for input,
    //but will not respect the timeout in rendering code
    template <typename Duration>
    void runWithTimeout(Duration);
};

END_PYRAMID_GL_NAMESPACE
