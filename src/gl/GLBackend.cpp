#include "gl/GLBackend.hpp"

#include "Util/Util.hpp"

#include "Util/Strcat.hpp"
#include "gl/GLWindow.hpp"

#include <thread>
#include <functional>
#include <atomic>
#include <iostream>

#include <spdlog/spdlog.h>

BEGIN_PYRAMID_GL_NAMESPACE

class GLWorkThread
{
    std::thread workThread;
    std::atomic_bool interrupted;

    void run(
            WorkQueue& workQueue,
            const Config::BackendOptions::GLBackendOptions::GraphLayoutOptions& layoutOptions,
            const Config::BackendOptions::GLBackendOptions::WindowOptions& windowOptions)
    {
        //delay OpenGL initialization until we get work
        std::unique_ptr<GLContext> glContext;

        std::shared_ptr<spdlog::logger> logger;
        //bootstrap spdlog with cerr
        try {
            logger = (spdlog::stdout_color_mt(STRCAT("GLWorkThread_", workThread.get_id())));
        }
        catch (const spdlog::spdlog_ex& ex)
        {
            std::cerr << "Error while initializing GLWorkThread: log init failed " 
                << ex.what() << std::endl;
            throw ex;
        }

        std::shared_ptr<GLWindow::EventPoller> eventPoller;

        //the user can exit by interacting with the SDL window
        //(e.g. pressing escape)
        bool receivedQuitEvent = false;


        //the draw loop
        //here we do 3 things:
        //
        //  1. (in the loop condition): check exit flags
        //  2. check for any new work
        //  3. initialize opengl state if needed
        //  4. update exit flags
        //  5. draw a frame
        //
        while(!interrupted.load() && !receivedQuitEvent)
        {
            //check if there's new work in the queue
            std::shared_ptr<Simulation::Backend::Data> task;

            {
                std::unique_ptr<const std::shared_ptr<Simulation::Backend::Data>> ret = 
                    workQueue.pop_return_back_or_null();

                if(ret != nullptr)
                {
                    task = std::shared_ptr<Simulation::Backend::Data>(*ret);
                }
            }
            
            if(task != nullptr)
            {
                //if there's a new task,
                
                //initialize opengl state if we haven't already
                if(glContext == nullptr)
                {
                    std::pair<std::unique_ptr<GraphLayout::Graph>, GraphLayout::BoundingBox> 
                        layout = GraphLayout(layoutOptions,
                            *task->graph).calculateLayout();

                    glContext = make_unique<GLContext>(windowOptions, layout);
                }

                if(eventPoller == nullptr)
                {
                    eventPoller = glContext->getEventPoller();
                }

                //TODO: handle any updates from the new task
            }


            //check if it's time to shutdown
            if(eventPoller->getEvent() == GLWindow::EventPoller::EventType::QUIT)
            {
                receivedQuitEvent = true;
                logger->info("Received quit event from GLWindow::EventPoller");
                //might as well skip this run
                continue;
            }

            //draw if OpenGL has been initialized
            if(glContext != nullptr)
            {
                glContext->drawAndSwapFrames();
            }
        }
    }

public:
    GLWorkThread(
            const Config::BackendOptions::GLBackendOptions::GraphLayoutOptions& layoutOptions,
            const Config::BackendOptions::GLBackendOptions::WindowOptions& windowOptions,
            WorkQueue& workQueue)
    {
        //need to initialize the flag before starting the thread
        interrupted.store(false);

        //launch the thread
        workThread = std::thread(
                std::bind(&GLWorkThread::run,
                    this, 
                    std::placeholders::_1, 
                    std::placeholders::_2,
                    std::placeholders::_3),
                std::ref(workQueue), std::ref(layoutOptions), std::ref(windowOptions));
    }
};


GLBackend::GLBackend(
        const Config::BackendOptions::GLBackendOptions::WindowOptions& options)
{

}

END_PYRAMID_GL_NAMESPACE
