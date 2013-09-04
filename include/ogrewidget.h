
#ifndef OGREWIDGET_H
    #define OGREWIDGET_H

    #include <gtkmm.h>
    #include <glibmm/timer.h>

    #include <Ogre.h>

    #include "VideoManager.h"
    #include "ARTKDetector.h"

    class OgreWidget : public Gtk::Widget
    {
    public:
      OgreWidget();
      virtual ~OgreWidget();
      void set_rotation(int value);

    protected:
      void loadResources();
      void createScene();

      virtual void on_size_request(Gtk::Requisition* requisition);
      virtual void on_size_allocate(Gtk::Allocation& allocation);
      virtual void on_map();
      virtual void on_unmap();
      virtual void on_realize();
      virtual void on_unrealize();
      virtual bool on_expose_event(GdkEventExpose* event);
      virtual bool on_idle();

      virtual bool on_motion_notify_event(GdkEventMotion *event);
      virtual bool on_button_press_event(GdkEventButton *event);
      virtual bool on_button_release_event(GdkEventButton *event);

      Glib::RefPtr<Gdk::Window> mRefGdkWindow;

      Ogre::RenderWindow* mRenderWindow;
      Ogre::SceneManager* mSceneMgr;
      Ogre::Viewport* mViewport;
      Ogre::Camera* mCamera;
      VideoManager* _videoManager;
      ARTKDetector* _arDetector;
    };
    #endif
