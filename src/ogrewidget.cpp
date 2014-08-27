#include "ogrewidget.h"

#include <gdkmm/drawable.h>
    #include <gdkmm/general.h>
    #include <gdk/gdkx.h>
    #include <iostream>
    #include <cstring>
    #include <cmath>

    #ifdef WIN32
       #include <gdk/gdkwin32.h>
    #endif

    #define PI 3.14159265

    OgreWidget::OgreWidget() :
      Glib::ObjectBase("ogrewidget"),
      Gtk::Widget(),
      mRenderWindow(0), mSceneMgr(0), mViewport(0), mCamera(0)
    {
       set_flags(Gtk::NO_WINDOW);
      std::cout << "GType name: " << G_OBJECT_TYPE_NAME(gobj()) << std::endl;
      _center[0] = 0.0;  _center[1] = 0.0;
      _fin[0] = 0.0;    _fin[1] = 0.0;  frame = 0;
//      for (int i=0;i<5;i++)
//        _marcas[i] = _scene->getMarca(i);
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
         createOverlay();
         _videoManager = new VideoManager(1, 640, 480, mSceneMgr);
         _scene = new Scene(_videoManager);
         _scene->init();
         _scene->setAlto(480);
         _scene->setAncho(640);
         _arDetector = new ARTKDetector(640, 480, 100, _scene);
         _scene->setARTK(_arDetector);
         coor = new Coordinator(_scene);
         coor->init();
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
          //_videoManager->DrawCurrentFrame();
          if ((frame % 10 ==1) && (_scene->getGrid()>0)){
              for (int j=0;j<=_scene->getAlto()/_scene->getGrid();j++){
                for (int i=0;i<=_scene->getAncho()/_scene->getGrid();i++){
                  std::cout<<_scene->getMap(i,j);
                }
                std::cout<<""<<std::endl;
              }
          }
//          cout<<"OW::Frame:: "<<frame<<endl;
          _videoManager->DrawCurrentFrame(frame);
          if (_arDetector->detectMark(_videoManager->getCurrentFrameMat())){
              //for (int i = 2; i < 5; i++){
                  if (_scene->getMarca(4)->getVisible()){
                      _arDetector->getPosRot(pos, look, up, _scene->getMarca(4));
                      mCamera->setPosition(pos);
                      mCamera->lookAt(look);
                      mCamera->setFixedYawAxis(true, up);
                      mSceneMgr->getSceneNode("SinbadNode")->setVisible(true);
                  }else{
                      mSceneMgr->getSceneNode("SinbadNode")->setVisible(false);
                  }
              //}
          }


              //Marca marcas[5];
              //_scene->getMarcas(marcas);
//              if (_arDetector->detectMark(_videoManager->getCurrentFrameMat())){
//                  //_scene->setMarcas(marcas);
//                  //_scene->Actualizar();     // LLamamos a la escena para que se actualice
//                  //else  mSceneMgr->getEntity("Esfera")->setVisible(false);
//              }

          //}
          frame ++;

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

    void OgreWidget::createOverlay() {
      mOverlayMgr = Ogre::OverlayManager::getSingletonPtr();
//      Ogre::Overlay *overlay = mOverlayMgr->getByName("Info");
//      overlay->show();
    }

    void OgreWidget::createScene()
    {
        // Set default mipmap level & texture filtering
        Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
        Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(Ogre::TFO_TRILINEAR);

        // Create scene manager
        mSceneMgr = Ogre::Root::getSingletonPtr()->createSceneManager(Ogre::ST_GENERIC, "SceneManager");

        // Create the mCameraera
        mCamera = mSceneMgr->createCamera("Camera");
        mCamera->setPosition(Ogre::Vector3(50,250,250));
        mCamera->lookAt(Ogre::Vector3(0,0,0));
        mCamera->setNearClipDistance(5);
        mCamera->setFarClipDistance(10000);
        mCamera->setFOVy(Ogre::Degree(48));

        // Create one viewport, entire window
        mViewport = mRenderWindow->addViewport(mCamera);

        mViewport ->setBackgroundColour(Ogre::ColourValue(0.0,0.0,0.0));

        // Alter the camera aspect ratio to match the viewport
        mCamera->setAspectRatio(Ogre::Real(mViewport->getActualWidth()) / Ogre::Real(mViewport->getActualHeight()));

        Ogre::Entity* ent1 = mSceneMgr->createEntity("Sinbad","Sinbad.mesh");
          Ogre::SceneNode* node1 = mSceneMgr->createSceneNode("SinbadNode");

          node1->attachObject(ent1);
          node1->setScale(2,2,2);
          node1->translate(Ogre::Vector3(0,0,0), Ogre::Node::TS_LOCAL);
          node1->setVisible(false);
          mSceneMgr->getRootSceneNode()->addChild(node1);

        mSceneMgr->setShadowTechnique(Ogre::SHADOWTYPE_STENCIL_ADDITIVE);
          Ogre::Light* light = mSceneMgr->createLight("Light1");
          light->setType(Ogre::Light::LT_DIRECTIONAL);
          light->setDirection(Ogre::Vector3(1,-1,0));
          node1->attachObject(light);

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
       if (event->button == 3){
         std::cout << "button right release "<<event->x/_scene->getGrid()<<":"<<event->y/_scene->getGrid()<<std::endl;
              Ogre::Overlay *overlay = mOverlayMgr->getByName("InfoRG");
              overlay->hide();
              overlay = mOverlayMgr->getByName("InfoOS");
              overlay->hide();
       }else if (event->button == 1){
           if (_scene->getPixel(event->x,event->y)!=-1){
               if (_scene->getPixel(event->x,event->y)<3){
                  Ogre::Overlay *overlay = mOverlayMgr->getByName("InfoRG");
                  overlay->show();
                  Ogre::OverlayElement *oe;
                  oe = mOverlayMgr->getOverlayElement("esquina");
                  oe->setCaption(" inferior");
                  oe = mOverlayMgr->getOverlayElement("direccion");
                  oe->setCaption(" derecha");
//                  switch(coor->getARobot(_scene->getPixel(event->x,event->y))->getGuardia()){
//                      case 1:
//                          oe = mOverlayMgr->getOverlayElement("esquina");
//                          oe->setCaption(Ogre::StringConverter::toString("inferior"));
//                          oe = mOverlayMgr->getOverlayElement("direccion");
//                          oe->setCaption(Ogre::StringConverter::toString("derecha"));
//                      break;
//                      case 2:
//                          oe = mOverlayMgr->getOverlayElement("esquina");
//                          oe->setCaption(Ogre::StringConverter::toString("superior"));
//                          oe = mOverlayMgr->getOverlayElement("direccion");
//                          oe->setCaption(Ogre::StringConverter::toString("derecha"));
//                      break;
//                      case 3:
//                          oe = mOverlayMgr->getOverlayElement("esquina");
//                          oe->setCaption(Ogre::StringConverter::toString("superior"));
//                          oe = mOverlayMgr->getOverlayElement("direccion");
//                          oe->setCaption(Ogre::StringConverter::toString("izquierda"));
//                      break;
//                      case 4:
//                          oe = mOverlayMgr->getOverlayElement("esquina");
//                          oe->setCaption(Ogre::StringConverter::toString("inferior"));
//                          oe = mOverlayMgr->getOverlayElement("direccion");
//                          oe->setCaption(Ogre::StringConverter::toString("izquierda"));
//                      break;
//                  }
               }else{
                  Ogre::Overlay *overlay = mOverlayMgr->getByName("InfoOS");
                  overlay->show();
               }
           }
           std::cout << "button left release "<<event->x<<":"<<event->y<<std::endl;
       }
       return true;
    }

    float OgreWidget::getRotacion(int id){
        Ogre::Vector3 pos;  Ogre::Vector3 look;   Ogre::Vector3 up;
        _arDetector->getPosRot(pos, look, up,_scene->getMarca(id));////////////////////////////////////////////////////////////////////////////////////////////
        float mod = atan((-up[0])/up[2]) - PI;
        if (up[2]<0) mod = mod + PI;
        else if ((-up[0])<0) mod = mod + 2 * PI;
        if (mod<0) mod = mod + 2 * PI;
        return mod * 180 / PI;
    }

    float OgreWidget::getAngulo(float v[2],float v1[2]){
//        float dividendo = v[0]*v1[0]+v[1]*v1[1];
//        float divisor = sqrt(v[0]*v[0]+v[1]*v[1])*sqrt(v1[0]*v1[0]+v1[1]*v1[1]);
//        float angulo = acos(dividendo/divisor);
//        return angulo * 180 / PI;
        v[1] = -v[1];
        float mod = atan((v[1])/v[0]);// - PI;
        if (v[0]<0) mod = mod + PI;
        else if ((v[1])<0) mod = mod + 2 * PI;
        //if (mod<0) mod = mod + 2 * PI;
        return mod * 180 / PI;
    }
