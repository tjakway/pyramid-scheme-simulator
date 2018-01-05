//the below SDL code is derived from the tutorial at http://headerphile.com/sdl2/opengl-part-1-sdl-opengl-awesome/
//can also be found at https://gist.githubusercontent.com/olevegard/e1ce53d9302c4ed3be8d/raw/8c61a54f29694ff0fb0013d473849cb7a84f6e68/Simple%20OpenGL%20%20test.cpp
//header reproduced below:
    // Headerphile.com OpenGL Tutorial part 1
    // A Hello World in the world of OpenGL ( creating a simple windonw and setting background color )
    // Source code is an C++ adaption / simplicication of : https://www.opengl.org/wiki/Tutorial1:_Creating_a_Cross_Platform_OpenGL_3.2_Context_in_SDL_(C_/_SDL)

#include "gl/GLWindow.hpp"

#include <string>
#include <atomic>
#include <iostream>
#include <cassert>

#include <GL/glew.h>
#include <SDL2/SDL.h>

#include "Util/Util.hpp"
#include "Util/Strcat.hpp"
#include "Util/NewExceptionType.hpp"

namespace {
    std::string getSDLErrorString()
    {
        using namespace pyramid_scheme_simulator;
        return Util::StringTrim::trim_copy(std::string(SDL_GetError()));
    }
}

BEGIN_PYRAMID_GL_NAMESPACE


class GLWindow::SDLGLHandle
{
    SDL_Window* window;

    /** ***XXX WARNING*** 
     * DO *NOT* COPY THE SDL_GLContext
     * IT *WILL* CAUSE THE WINDOW TO CRASH
     * AND WILL NOT TELL YOU WHY
     * *ONLY*, **ONLY** MOVE IT 
     * AND ONLY THEN IF YOU REALLY KNOW WHAT YOU'RE DOING */
    SDL_GLContext& glContext;

public:
    SDL_Window* getWindow() { return window; }
    SDL_GLContext& getGLContext() { return glContext; }

    //move semantics only!
    SDLGLHandle(SDL_Window* _window,
            SDL_GLContext&& _glContext)
        : window(_window),
        glContext(_glContext)
    {}

    SDLGLHandle(SDLGLHandle&& other)
        : window(other.window),
        glContext(other.glContext)
    {}

    virtual ~SDLGLHandle()
    {
        //even though there's a page for this I *really* don't think we're
        //supposed to call it... I get segfaults on Ubuntu 16.04.3
        //with Nvidia drivers when I call it
        //SDL_GL_DeleteContext(glContext);
        
        if(window)
        {
            SDL_DestroyWindow(window);
        }
    }
};

class GLWindow::SDL
{
private:
    //global flag tracking whether we've initialized SDL
    //to prevent trying to initialize it twice
    static std::atomic_bool sdlInitialized;

    NEW_EXCEPTION_TYPE_WITH_BASE(SDLException, GLWindowException);
    NEW_EXCEPTION_TYPE_WITH_BASE(SDLInitException, SDLException);
    NEW_EXCEPTION_TYPE_WITH_BASE(SDLCreateWindowException, SDLInitException);
    NEW_EXCEPTION_TYPE_WITH_BASE(SDLCreateGLContextException, SDLInitException);

    static void throwIfSDLError()
    {
#ifndef GL_WINDOW_SKIP_SDL_ERROR_CHECKING
        const std::string errMsg = getSDLErrorString();
        if(errMsg != std::string("") && !errMsg.empty())
        {
            SDL_ClearError();
            throw SDLException(STRCAT("SDL error: ", errMsg));
        }
#endif
    }

    static void setOpenGLAttributes();

    static void initSDL()
    {
        if(!sdlInitialized.load())
        {
            if (SDL_Init(SDL_INIT_VIDEO) < 0)
            {
                throw SDLInitException("Failed to init SDL with SDL_INIT_VIDEO");
            }
            else
            {
                sdlInitialized.store(true);
            }
        }

        assert(sdlInitialized.load());
    }

    static SDL_Window* createWindow(
            const std::string& title, 
            std::pair<int, int> windowDimensions)
    {
        //make sure SDL has been set up
        initSDL();

        SDL_Window* win = SDL_CreateWindow(title.c_str(),
                SDL_WINDOWPOS_CENTERED,
                SDL_WINDOWPOS_CENTERED,
		windowDimensions.first,
                windowDimensions.second, 
                SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

        if(!win)
        {
            throw SDLCreateWindowException(__func__);
        }

        return win;
    }

    static SDL_GLContext createGLContext(
            SDL_Window* win, 
            int majorVersion, 
            int minorVersion)
    {
        assert(win != nullptr);

        SDL_GLContext context = SDL_GL_CreateContext(win);

        if(!context)
        {
            throw SDLCreateGLContextException(__func__);
        }

        if(!SDL_GL_MakeCurrent(win, context))
        {
            throwIfSDLError();
        }


        setOpenGLAttributes(majorVersion, minorVersion);
        return std::move(context);
    }

    static void setOpenGLAttributes(int majorVersion, int minorVersion)
    {
        //TODO: do I need this?
	SDL_GL_SetSwapInterval(1);

        //use core profile
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        if(majorVersion >= 1 && minorVersion >= 1)
        {
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, majorVersion);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minorVersion);
        }

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    }

public:

    static std::unique_ptr<SDLGLHandle> makeSDLGLWindow(
            const std::string& title, 
            std::pair<int, int> windowDimensions,
            int openglRequiredMajorVersion,
            int openglRequiredMinorVersion)
    {
        SDL_Window* win = createWindow(title, windowDimensions);

        return make_unique<SDLGLHandle>(
                win,
                createGLContext(
                    win,
                    openglRequiredMajorVersion,
                    openglRequiredMinorVersion));
    }

    static void runLoop(SDLGLHandle* handle, 
            std::function<void()> _init,  
            std::function<void()> _draw,
            std::function<void()> _cleanup)
    {
        //ensure that this OpenGL context is ready to go
        if(!SDL_GL_MakeCurrent(handle->getWindow(), handle->getGLContext()))
        {
            throwIfSDLError();
        }

        _init();

        //need this flag to be able to break out of the nested loop
        bool loop = true;
	while(loop)
	{
            //should we draw first then poll or poll then draw?
            _draw();


            SDL_Event event;
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT)
                {
                    loop = false;
                }

                if (event.type == SDL_KEYDOWN)
                {
                        switch (event.key.keysym.sym)
                        {
                        case SDLK_ESCAPE:
                                loop = false;
                                break;
                        default:
                                break;
                        }
                }

                //TODO: should we swap windows here?
                //alternatively, just do it in the render loop
                //SDL_GL_SwapWindow(handle.window);
            }

            SDL_GL_SwapWindow(handle->getWindow());
	}

        _cleanup();
    }
};

std::atomic_bool GLWindow::SDL::sdlInitialized {false};


GLWindow::GLWindow(const std::string& title, 
    std::pair<int, int> windowDimensions,
    std::function<void()> _init,
    std::function<void()> _draw,
    std::function<void()> _cleanup,
    int openglRequiredMajorVersion, //ignored if <1
    int openglRequiredMinorVersion) //ignored if <1
    : init(_init), 
    draw(_draw), 
    cleanup(_cleanup)
{
    sdlGlHandle = GLWindow::SDL::makeSDLGLWindow(title,
            windowDimensions, 
            openglRequiredMajorVersion,
            openglRequiredMinorVersion);
}

void GLWindow::run()
{
    GLWindow::SDL::runLoop(
            sdlGlHandle.get(),
            init,
            draw,
            cleanup);
}

GLWindow::~GLWindow() 
{ }

END_PYRAMID_GL_NAMESPACE
