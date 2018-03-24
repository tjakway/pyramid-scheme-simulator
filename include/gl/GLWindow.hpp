#pragma once


#include <string>
#include <functional>
#include <iostream>
#include <memory>
#include <mutex>
#include <deque>
#include <thread>

#include "gl/GLUtil.hpp"
#include "Util/NewExceptionType.hpp"
#include "NamespaceDefines.hpp"

BEGIN_PYRAMID_GL_NAMESPACE


//NOTE: #define GL_WINDOW_SKIP_SDL_ERROR_CHECKING
//to skip SDL error checking

class GLWindow
{
protected:
    class SDL;
    class SDLGLHandle;

    std::unique_ptr<SDLGLHandle> sdlGlHandle;

public:
    NEW_EXCEPTION_TYPE(GLWindowException);

    class EventPoller
    {
    public:
        EventPoller();

        //mirrors SDL equivalent
        enum EventType
        {
            //add more in the future
            NO_EVENT, QUIT
        };

        EventType getEvent();

    private:
        using LockType = std::lock_guard<std::recursive_mutex>;
        std::deque<EventType> eventQueue;
        std::recursive_mutex eventQueueMutex;

        void poll();
        void addEvent(EventType);
        std::thread pollThread;
    };

    GLWindow(const std::string& title, 
            std::pair<int, int> windowDimensions,
            int openglRequiredMajorVersion = -1, //ignored if <1
            int openglRequiredMinorVersion = -1);//ignored if <1

    virtual ~GLWindow();

    std::shared_ptr<EventPoller> getEventPoller();

    void makeCurrent();
    void swapWindow();

protected:
    std::shared_ptr<EventPoller> eventPoller;
};

END_PYRAMID_GL_NAMESPACE
