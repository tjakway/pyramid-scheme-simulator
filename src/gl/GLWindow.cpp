#include "gl/GLWindow.hpp"

#include <SDL.h>
#include "Util/NewExceptionType.hpp"

BEGIN_PYRAMID_GL_NAMESPACE

class GLWindow::SDL
{
private:
    //global flag tracking whether we've initialized SDL
    //to prevent trying to initialize it twice
    static std::atomic_bool sdlInitialized;

public:
    NEW_EXCEPTION_TYPE_WITH_BASE(SDLInitException, GLWindowInitException);

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
