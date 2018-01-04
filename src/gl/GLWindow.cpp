#include "gl/GLWindow.hpp"

#include <string>

#include <SDL.h>
#include "Util/NewExceptionType.hpp"

BEGIN_PYRAMID_GL_NAMESPACE

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

    void setOpenGLAttributes();

    void initSDL()
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
    }

    SDL_Window* createWindow(
            const std::string& title, 
            std::pair<int, int> windowDimensions)
    {
        SDL_Window* win = SDL_CreateWindow(title.c_str(),
                SDL_WINDOWPOS_CENTERED,
                SDL_WINDOWPOS_CENTERED,
		windowDimensions.first,
                windowDimensions.second, 
                SDL_WINDOW_OPENGL);

        if(!win)
        {
            throw SDLCreateWindowException(__func__);
        }

        return win;
    }

    SDL_GLContext createGLContext(SDL_Window* win)
    {
        SDL_GLContext context = SDL_GL_CreateContext(win);

        if(!context)
        {
            throw SDLCreateGLContextException(__func__);
        }

        setOpenGLAttributes();

        return context;
    }

    void setOpenGLAttributes(int majorVersion, int minorVersion)
    {
        //TODO: do I need this?
	SDL_GL_SetSwapInterval(1);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

        if(majorVersion >= 1 && minorVersion >= 1)
        {
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, majorVersion);
            SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, minorVersion);
        }

	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
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

    if(openglRequiredMajorVersion >= 1 && openglRequiredMinorVersion >= 1)
    {
    }
}


END_PYRAMID_GL_NAMESPACE
