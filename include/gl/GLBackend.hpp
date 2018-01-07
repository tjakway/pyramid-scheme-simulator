#pragma once

#include "NamespaceDefines.hpp"
#include "Simulation.hpp"
#include "Util/AtomicDeque.hpp"

#include "gl/GLContext.hpp"

#include <memory>
#include <mutex>

BEGIN_PYRAMID_GL_NAMESPACE

using WorkQueue = AtomicDeque<const std::shared_ptr<Simulation::Backend::Data>, 
      //don't need a recursive mutex
      std::mutex>;

class GLWorkThread;

class GLBackend : public Simulation::Backend
{
    WorkQueue workQueue;
    bool builtLayout = false;

    std::unique_ptr<GLContext> glContext;

public:
    virtual void exportData(const std::shared_ptr<Simulation::Backend::Data>) override;

    GLBackend(const Config::BackendOptions::GLBackendOptions::WindowOptions&);
};

END_PYRAMID_GL_NAMESPACE