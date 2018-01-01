#pragma once

#include <GL/glew.h>
#include <gtkmm.h>

#include <string>
#include <functional>
#include <iostream>

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
        mGlArea.throw_if_error();
    }

    void onUnrealize() {
        cleanup();
        mGlArea.throw_if_error();
    }

    bool onRender(const Glib::RefPtr<Gdk::GLContext>& /*context*/) {
        mGlArea.throw_if_error();
        draw();
        return true;
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

        const int width = windowDimensions.first,
              height = windowDimensions.second;

        set_default_size(width, height);

        Gtk::Box vBox {Gtk::Orientation::ORIENTATION_VERTICAL, false};
        add(vBox);

        mGlArea.set_auto_render(true);
        mGlArea.set_hexpand();
        mGlArea.set_vexpand();
        mGlArea.set_halign(Gtk::ALIGN_FILL);
        mGlArea.set_valign(Gtk::ALIGN_FILL);


        if(openglRequiredMajorVersion >= 1 && openglRequiredMinorVersion >= 1)
        {
            mGlArea.set_required_version(
                    openglRequiredMajorVersion, 
                    openglRequiredMinorVersion);
        }

        vBox.add(mGlArea);

        //setup callbacks
        mGlArea.signal_realize().connect(sigc::mem_fun(this, 
        &GLWindow::onRealize));
        mGlArea.signal_unrealize().connect(sigc::mem_fun(this, 
        &GLWindow::onUnrealize), false);
        mGlArea.signal_render().connect(sigc::mem_fun(this, 
        &GLWindow::onRender), false);

        mGlArea.make_current();
        assert(!mGlArea.has_error());
        try {
        mGlArea.throw_if_error();
        }
        catch(const Gdk::GLError& gle)
        {
            std::cerr << "An error occured making the context current during CTOR" << std::endl;
            std::cerr << gle.domain() << "-" << gle.code() << "-" << gle.what() << std::endl;
        }

//        add(mGlArea);
//        this->queue_draw();
    }

    virtual ~GLWindow() {}

    void run(const Glib::RefPtr<Gtk::Application>& app)
    {
        show();
        app->run(*this);
    }
};

END_PYRAMID_GL_NAMESPACE
