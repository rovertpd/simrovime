#include "ogrewidget.h"

#include <gdkmm/drawable.h>
    #include <gdkmm/general.h>
    #include <gdk/gdkx.h>
    #include <iostream>
    #include <cstring>

    #ifdef WIN32
       #include <gdk/gdkwin32.h>
    #endif

    OgreWidget::OgreWidget() :
      Glib::ObjectBase("ogrewidget"),
      Gtk::Widget(),
      mRenderWindow(0), mCamera(0), mSceneMgr(0), mViewport(0)
    {
      set_flags(Gtk::NO_WINDOW);
      std::cout << "GType name: " << G_OBJECT_TYPE_NAME(gobj()) << std::endl;
    }
    void OgreWidget::set_rotation(int value)
    {
        Ogre::SceneNode *node = mSceneMgr->getSceneNode("Esfera");
        node->yaw(Ogre::Degree(value));
    }
    OgreWidget::~OgreWidget()
    {
    }

    void OgreWidget::on_size_request(Gtk::Requisition* requisition)
    {
      *requisition = Gtk::Requisition();

      requisition->width = 800;
      requisition->height = 600;
    }

    void OgreWidget::on_size_allocate(Gtk::Allocation& allocation)
    {
      set_allocation(allocation);

      if(mRefGdkWindow)
      {
        mRefGdkWindow->move_resize( allocation.get_x(), allocation.get_y(),
                allocation.get_width(), allocation.get_height() );
      }

      if (mRenderWindow)
      {
          mRenderWindow->windowMovedOrResized();
          mCamera->setAspectRatio(Ogre::Real(allocation.get_width()) / Ogre::Real(allocation.get_height()));
          on_expose_event(NULL);
      }
    }

    void OgreWidget::on_map()
    {
      //Call base class:
      Gtk::Widget::on_map();
    }

    void OgreWidget::on_unmap()
    {
      //Call base class:
      Gtk::Widget::on_unmap();
    }

    void OgreWidget::on_realize()
    {
       //Call base class:
       Gtk::Widget::on_realize();

       Gtk::Allocation allocation = get_allocation();

       if(!mRefGdkWindow)
       {
          //Create the GdkWindow:
          GdkWindowAttr attributes;
          memset(&attributes, 0, sizeof(attributes));

          //Set initial position and size of the Gdk::Window:
          attributes.x = allocation.get_x();
          attributes.y = allocation.get_y();
          attributes.width = allocation.get_width();
          attributes.height = allocation.get_height();

          attributes.event_mask = get_events () | Gdk::EXPOSURE_MASK | Gdk::ALL_EVENTS_MASK ;
          attributes.window_type = GDK_WINDOW_CHILD;
          attributes.wclass = GDK_INPUT_OUTPUT;

          mRefGdkWindow = Gdk::Window::create(get_window(), &attributes, GDK_WA_X | GDK_WA_Y);

       }

       if (!mRenderWindow)
       {
          Ogre::NameValuePairList params;
    #ifdef WIN32
       params["externalWindowHandle"] = Ogre::StringConverter::toString((unsigned long)GDK_WINDOW_HWND(mRefGdkWindow->gobj()));
    #else
    GdkWindow* parent = mRefGdkWindow->gobj();
       GdkDisplay* display = gdk_drawable_get_display(GDK_DRAWABLE(parent));
       GdkScreen* screen = gdk_drawable_get_screen(GDK_DRAWABLE(parent));

    Display* xdisplay = GDK_DISPLAY_XDISPLAY(display);
       Screen* xscreen = GDK_SCREEN_XSCREEN(screen);
       int screen_number = XScreenNumberOfScreen(xscreen);
       XID xid_parent = GDK_WINDOW_XWINDOW(parent);

       params["externalWindowHandle"] =
          Ogre::StringConverter::toString(reinterpret_cast<unsigned long>(xdisplay)) + ":" +
          Ogre::StringConverter::toString(static_cast<unsigned int>(screen_number)) + ":" +
          Ogre::StringConverter::toString(static_cast<unsigned long>(xid_parent));
    #endif
          mRenderWindow = Ogre::Root::getSingleton().createRenderWindow("Gtk+Ogre Widget",
             allocation.get_width(), allocation.get_height(), false, &params);

         mRenderWindow->setAutoUpdated(false);

        unset_flags(Gtk::NO_WINDOW);

         set_window(mRefGdkWindow);

          set_double_buffered(true);
          //make the widget receive expose events
         mRefGdkWindow->set_user_data(gobj());
         mRefGdkWindow->set_back_pixmap(Glib::RefPtr<Gdk::Pixmap>(),false);
         loadResources();
         createScene();
         _videoManager = new VideoManager(1, 640, 480, mSceneMgr);
         _arDetector = new ARTKDetector(640, 480, 100);
          // Start idle function for frame update/rendering
          Glib::signal_idle().connect( sigc::mem_fun(*this, &OgreWidget::on_idle) );

       }
    }

    void OgreWidget::on_unrealize()
    {
      mRefGdkWindow.clear();
      //Call base class:
      Gtk::Widget::on_unrealize();
    }

    bool OgreWidget::on_expose_event(GdkEventExpose* event)
    {

      if (mRenderWindow)
      {
          Ogre::Root::getSingletonPtr()->_fireFrameStarted();
          mRenderWindow->update();
          Ogre::Vector3 pos;  Ogre::Vector3 look;   Ogre::Vector3 up;

          _videoManager->UpdateFrame();
          _videoManager->DrawCurrentFrame();
          if (_arDetector->detectMark(_videoManager->getCurrentFrameMat())){
            std::cout<<"Marca encontrada"<<std::endl;
            _arDetector->getPosRot(pos, look, up);
            mCamera->setPosition(pos);
            mCamera->lookAt(look);
            mCamera->setFixedYawAxis(true, up);

            Ogre::Entity* ent2 = mSceneMgr->createEntity("Esfera", "Esfera.mesh");
            Ogre::SceneNode* node2 = mSceneMgr->createSceneNode("Esfera");
            ent2->setMaterialName("Material2");
            node2->attachObject(ent2);
            mSceneMgr->getRootSceneNode()->addChild(node2);


          }
          Ogre::Root::getSingletonPtr()->_fireFrameEnded();

      }
      return true;
    }

    bool OgreWidget::on_idle()
    {
        on_expose_event(0);
        return true;
    }

    void OgreWidget::loadResources() {
      Ogre::ConfigFile cf;
      cf.load("resources.cfg");

      Ogre::ConfigFile::SectionIterator sI = cf.getSectionIterator();
      Ogre::String sectionstr, typestr, datastr;
      while (sI.hasMoreElements()) {
        sectionstr = sI.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = sI.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i) {
          typestr = i->first;    datastr = i->second;
          Ogre::ResourceGroupManager::getSingleton().addResourceLocation
                (datastr, typestr, sectionstr);
        }
      }
      Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
    }

    void OgreWidget::createScene()
    {
        // Set default mipmap level & texture filtering
        //Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
        //Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(Ogre::TFO_TRILINEAR);

        // Create scene manager
        mSceneMgr = Ogre::Root::getSingletonPtr()->createSceneManager(Ogre::ST_GENERIC, "SceneManager");

        // Create the mCameraera
        mCamera = mSceneMgr->createCamera("Camera");
        mCamera->setPosition(Ogre::Vector3(5,10.5,12));
        mCamera->lookAt(Ogre::Vector3(1.4,4.3,3));
        mCamera->setNearClipDistance(5);
        mCamera->setFarClipDistance(10000);
        mCamera->setFOVy(Ogre::Degree(48));

        // Create one viewport, entire window
        mViewport = mRenderWindow->addViewport(mCamera);

        mViewport ->setBackgroundColour(Ogre::ColourValue(0.0,0.0,0.0));

        // Alter the camera aspect ratio to match the viewport
        mCamera->setAspectRatio(Ogre::Real(mViewport->getActualWidth()) / Ogre::Real(mViewport->getActualHeight()));

        /*Ogre::Entity* ent2 = mSceneMgr->createEntity("Esfera", "Esfera.mesh");
        Ogre::SceneNode* node2 = mSceneMgr->createSceneNode("Esfera");
        ent2->setMaterialName("Material2");
        node2->attachObject(ent2);
        mSceneMgr->getRootSceneNode()->addChild(node2);
*/
        mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
        mSceneMgr->setAmbientLight(Ogre::ColourValue(0.2, 0.2, 0.2));

        Ogre::Light* light = mSceneMgr->createLight("Light1");
        light->setType(Ogre::Light::LT_DIRECTIONAL);
        light->setDirection(Ogre::Vector3(1,-1,0));

        Ogre::Light* light2 = mSceneMgr->createLight("Light2");
        light2->setType(Ogre::Light::LT_POINT);
        light2->setPosition(8, 8, -2);
        light2->setSpecularColour(0.9, 0.9, 0.9);
        light2->setDiffuseColour(0.9, 0.9, 0.9);

        /*Ogre::Plane plane1(Ogre::Vector3::UNIT_Y, 0);
        Ogre::MeshManager::getSingleton().createPlane("plane1",
        Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, plane1,
        200,200,1,1,true,1,10,10,Ogre::Vector3::UNIT_Z);

        Ogre::SceneNode* node3 = mSceneMgr->createSceneNode("ground");
        Ogre::Entity* groundEnt = mSceneMgr->createEntity("planeEnt", "plane1");
        groundEnt->setMaterialName("Ground");
        node3->attachObject(groundEnt);
        mSceneMgr->getRootSceneNode()->addChild(node3);
*/
        //_framelistener = new MyFrameListener(mRenderWindow, mCamera, mSceneMgr);
        //Ogre::Root::getSingletonPtr()->addFrameListener(_framelistener);

        //Ogre::Root::getSingletonPtr()->startRendering();
    }

    bool OgreWidget::on_motion_notify_event(GdkEventMotion *event) {
       std::cout << ".";
       return true;
    }

    bool OgreWidget::on_button_press_event(GdkEventButton *event) {
       std::cout << "button press\n";
       return true;
    }

    bool OgreWidget::on_button_release_event(GdkEventButton *event) {
       std::cout << "button release\n";
       return true;
    }
