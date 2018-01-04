#pragma once

#include <GL/glew.h>

#include <string>
#include <functional>
#include <iostream>
#include <memory>

#include "gl/GLUtil.hpp"
#include "Util/NewExceptionType.hpp"
#include "NamespaceDefines.hpp"

BEGIN_PYRAMID_GL_NAMESPACE


#ifdef GL_WINDOW_SKIP_SDL_ERROR_CHECKING
#define GL_WINDOW_SKIP_SDL_ERROR_CHECKING_VALUE true
#else
#define GL_WINDOW_SKIP_SDL_ERROR_CHECKING_VALUE false
#endif

class GLWindow
{
protected:
    class SDL;
    class SDLGLHandle;

    std::unique_ptr<SDLGLHandle> sdlGlHandle;


    /**
     * the functions passed by GLContext
     */
    const std::function<void()> init, draw, cleanup;

public:
    NEW_EXCEPTION_TYPE(GLWindowException);

    GLWindow(const std::string& title, 
            std::pair<int, int> windowDimensions,
            std::function<void()> _init,
            std::function<void()> _draw,
            std::function<void()> _cleanup,
            int openglRequiredMajorVersion = -1, //ignored if <1
            int openglRequiredMinorVersion = -1);//ignored if <1

    virtual ~GLWindow() {}

    void run();
};

END_PYRAMID_GL_NAMESPACE
