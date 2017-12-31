#pragma once


#include "NamespaceDefines.hpp"

#include <string>
#include <memory>

BEGIN_PYRAMID_GL_NAMESPACE

class GLContext
{
protected:
    static const std::string vertexShaderSource,
                             fragmentShaderSource;

    static const std::string windowName;
    
    struct WindowData;
    std::unique_ptr<WindowData> windowData;

public:

    GLContext(int argc, char** argv);

    void run();
};

END_PYRAMID_GL_NAMESPACE
