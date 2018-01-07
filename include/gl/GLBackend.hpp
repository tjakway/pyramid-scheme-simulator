#pragma once

#include "NamespaceDefines.hpp"
#include "Simulation.hpp"
#include "Util/AtomicDeque.hpp"

#include "gl/GLContext.hpp"

#include <memory>
#include <mutex>

BEGIN_PYRAMID_GL_NAMESPACE

using WorkQueue = AtomicDeque<std::shared_ptr<Simulation::Backend::Data>, 
      //don't need a recursive mutex
      std::mutex>;

class GLWorkThread;

class GLBackend : public Simulation::Backend
{
    WorkQueue workQueue;

    std::unique_ptr<GLWorkThread> glWorkThread;

public:
    virtual void exportData(const std::shared_ptr<Simulation::Backend::Data>) override;

    GLBackend(
            const Config::BackendOptions::GLBackendOptions::GraphLayoutOptions& layoutOptions,
            const Config::BackendOptions::GLBackendOptions::WindowOptions& windowOptions);

    virtual ~GLBackend();

    virtual void interrupt() noexcept override;
    virtual bool interrupted() const noexcept override;

    virtual void join() noexcept override;
};

END_PYRAMID_GL_NAMESPACE
