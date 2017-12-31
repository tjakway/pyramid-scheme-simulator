#pragma once

#include <gtkmm.h>
#include <GL/glew.h>

#include <string>
#include <functional>

#include "gl/GLUtil.hpp"
#include "NamespaceDefines.hpp"

BEGIN_PYRAMID_GL_NAMESPACE


//see http://smokesr.blogspot.com/2015/11/gtkmm-glarea-with-glew.html
class GLWindow : public Gtk::Window 
{
protected:
    Gtk::GLArea mGlArea;

    /**
     * the functions passed by GLContext
     */
    const std::function<void()> init, draw, cleanup;


    void onRealize() {
        mGlArea.make_current();
        init();
    }

    void onUnrealize() {
        cleanup();
    }

    bool onRender(const Glib::RefPtr<Gdk::GLContext>& /*context*/) {
        draw();
    }

public:
    GLWindow(const std::string& title, 
            std::pair<int, int> windowDimensions,
            std::function<void()> _init,
            std::function<void()> _draw,
            std::function<void()> _cleanup,
            int openglRequiredMajorVersion = -1, //ignored if <1
            int openglRequiredMinorVersion = -1) //ignored if <1
        : init(_init), 
        draw(_draw), 
        cleanup(_cleanup)
    {
        set_title(title);

        add(mGlArea);
        mGlArea.set_auto_render();
        mGlArea.set_hexpand();
        mGlArea.set_vexpand();
        mGlArea.set_halign(Gtk::ALIGN_FILL);
        mGlArea.set_valign(Gtk::ALIGN_FILL);

        const int width = windowDimensions.first,
              height = windowDimensions.second;

        mGlArea.set_size_request(width, height);

        if(openglRequiredMajorVersion >= 1 && openglRequiredMinorVersion >= 1)
        {
            mGlArea.set_required_version(
                    openglRequiredMajorVersion, 
                    openglRequiredMinorVersion);
        }

        //setup callbacks
        mGlArea.signal_realize().connect(sigc::mem_fun(this, 
        &GLWindow::onRealize));
        mGlArea.signal_unrealize().connect(sigc::mem_fun(this, 
        &GLWindow::onUnrealize), false);
        mGlArea.signal_render().connect(sigc::mem_fun(this, 
        &GLWindow::onRender));
    }

    virtual ~GLWindow() {}

    void run(const Glib::RefPtr<Application>& app, 
            const char* applicationId)
    {
        app->run(*this);
    }
};

END_PYRAMID_GL_NAMESPACE
