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

    void glInit(std::pair<const GraphLayout::Graph&, GraphLayout::BoundingBox>);

public:
    GLContext(const Config::BackendOptions::GLBackendOptions::WindowOptions&,
            std::pair<const GraphLayout::Graph&, GraphLayout::BoundingBox>);

    std::shared_ptr<GLWindow::EventPoller> getEventPoller();

    void drawAndSwapFrames();
};

END_PYRAMID_GL_NAMESPACE
