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
      _center[0] = 0.0;  _center[1] = 0.0;  _scene = Scene::Instance();
      _fin[0] = 0.0;    _fin[1] = 0.0;  frame = 0;
      for (int i=0;i<5;i++)
        _marcas[i] = _scene->getMarca(i);
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
      double pattTrans[3][4];
//      double pattTrans2[3][4];
//      double m[3][4];
//      double m2[3][4];
      double dist01=0.0;
      double fin[2];

      if (mRenderWindow)
      {
//          double pattTrans[3][4];
          Ogre::Root::getSingletonPtr()->_fireFrameStarted();
          mRenderWindow->update();
          Ogre::Vector3 pos;  Ogre::Vector3 look;   Ogre::Vector3 up;
          _videoManager->UpdateFrame();
          _videoManager->DrawCurrentFrame();
          if (frame % 5 ==0){
              if (_arDetector->detectMark(_videoManager->getCurrentFrameMat())){
                if(_scene->getMarca(0)->getVisible() && _center[0]==0.0){
                    _scene->getMarca(0)->getPattTans(pattTrans);
                        if (pattTrans[0][3]>0){
                            _center[0] = pattTrans[0][3];
                            _center[1] = pattTrans[1][3];
                        }else{
                            _center[0] = pattTrans[0][3] * (-1);
                            _center[1] = pattTrans[1][3] * (-1);
                        }
                }
                if(_scene->getMarca(2)->getVisible() && _fin[0]==0.0 && _center[0]!=0.0){
                    _scene->getMarca(2)->getPattTans(pattTrans);
                    if (pattTrans[0][3]<0){
                        _fin[0] = pattTrans[0][3] * (-1) + _center[0];
                        _fin[1] = pattTrans[1][3] * (-1) + _center[1];
                    }else{
                        _fin[0] = pattTrans[0][3] + _center[0];
                        _fin[1] = pattTrans[1][3] + _center[1];
                    }
                    printf(" Coordenada de fin final:X %f, Y %f\n",_fin[0],_fin[1]);
                }

    //            if (_scene->getMarca(0)->getVisible() && _scene->getMarca(2)->getVisible()) {
    //                _scene->getMarca(0)->getPattTans(pattTrans);
    //                _scene->getMarca(2)->getPattTans(pattTrans2);
    //                arUtilMatInv(pattTrans, m);
    //                arUtilMatMul(m, pattTrans2, m2);
    //                dist01 = sqrt(pow(m2[0][3],2)+pow(m2[1][3],2)+pow(m2[2][3],2));
    //                printf ("Distancia objects[1] y objects[2]= %G\n", dist01);
    //              }

                if (_scene->getMarca(3)->getVisible() && _fin[0]!=0.0){
                    Robot* robot = _scene->getRobot(0);
                    std::cout<<robot->getDir()<<" "<<robot->getEst()<<std::endl;
                    float v[2];
                    float v1[2];
                    if (robot->getDir()==0){
                        robot->setDir(1);
                        fin[0] = _fin[0];
                        fin[1] = 0.0;
                        robot->setFin(fin);
                        robot->setEst(0);
                        _scene->getMarca(3)->getPattTans(pattTrans);
                        v[0] = fin[0] - (pattTrans[0][3] + _center[0]);
                        v[1] = fin[1] - (pattTrans[1][3] + _center[1]);
                        v1[0] = 1.0;
                        v1[1] = 0.0;
                        float angulo = getAngulo(v,v1);
                        robot->setRot(angulo);
                        if (angulo>getRotacion(3)){
                            if (angulo-getRotacion(3)<180){
                                robot->izquierda();
                                robot->izquierda();
                            }
                            else{
                                robot->derecha();
                                robot->derecha();
                            }
                        }else{
                            if (angulo-getRotacion(3)<180){
                                robot->derecha();
                                robot->derecha();
                            }
                            else{
                                robot->izquierda();
                                robot->izquierda();
                            }
                        }
                        robot->setEst(2);
                    }
                    _scene->getMarca(3)->getPattTans(pattTrans);
                    double m[2];
                    m[0] = pattTrans[0][3]+_center[0];
                    m[1] = pattTrans[1][3]+_center[1];
                    double m2[2];
                    robot->getFin(m2);


                    dist01 = sqrt(pow((m2[0]-m[0]),2)+pow((m2[1]-m[1]),2));
                    std::cout<<dist01<<std::endl;
                    std::cout<<getRotacion(3)<<" "<<robot->getRot()<<std::endl;
                    if (dist01>=100.0){
                        if(getRotacion(3)<(robot->getRot()-5) || getRotacion(3)>(robot->getRot()+5)){
                            if (robot->getEst()!=2){
                                _scene->getMarca(3)->getPattTans(pattTrans);
                                robot->getFin(m2);
                                v[0] = m2[0] - (pattTrans[0][3] + _center[0]);
                                v[1] = m2[1] - (pattTrans[1][3] + _center[1]);
                                v1[0] = 1.0;
                                v1[1] = 0.0;
                                //printf("Pos: %f, %f\n",(pattTrans[0][3] + _center[0]),(pattTrans[1][3] + _center[1]));
                                //printf("Fin: %f, %f\n",m2[0],m2[1]);
                                //printf("Vector1: %f, %f\n",v[0],v[1]);
                                //printf("Vector2: %f, %f\n",v1[0],v1[1]);
                                float angulo = getAngulo(v,v1);
                                //printf("Angulo: %f\n",angulo);
                                robot->setRot(angulo);
                                if (angulo>getRotacion(3)){
                                    if (angulo-getRotacion(3)<180){
                                        robot->izquierda();
                                    }
                                    else{
                                        robot->derecha();
                                    }
                                }else{
                                    if (angulo-getRotacion(3)<180){
                                        robot->derecha();
                                    }
                                    else{
                                        robot->izquierda();
                                    }
                                }
                                robot->setEst(2);
                            }else{
                                if (getRotacion(3)<(robot->getRot()+15) && getRotacion(3)>(robot->getRot()-15)){
                                    robot->setEst(1);
                                    robot->avanzar();
                                }else{
                                    if (robot->getRot()>getRotacion(3)){
                                        if (abs(robot->getRot()-getRotacion(3))<180){
                                            robot->izquierda();
                                        }
                                        else{
                                            robot->derecha();
                                        }
                                    }else{
                                        if (abs(robot->getRot()-getRotacion(3))<180){
                                            robot->derecha();
                                        }
                                        else{
                                            robot->izquierda();
                                        }
                                    }
                                }
                            }
                        }else{
                                robot->setEst(1);
                                robot->avanzar();
                        }
                    }else{
                        robot->setEst(0);
                        switch(robot->getDir()){
                            case 1:
                                robot->setDir(2);
                                fin[0] = _fin[0];
                                fin[1] = _fin[1];
                                robot->setFin(fin);
                            break;
                            case 2:
                                robot->setDir(3);
                                fin[0] = 0.0;
                                fin[1] = _fin[1];
                                robot->setFin(fin);
                            break;
                            case 3:
                                robot->setDir(4);
                                fin[0] = 0.0;
                                fin[1] = 0.0;
                                robot->setFin(fin);
                            break;
                            case 4:
                                robot->setDir(1);
                                fin[0] = _fin[0];
                                fin[1] = 0.0;
                                robot->setFin(fin);
                            break;
                        }
                    }




//                    _arDetector->getPosRot(pos, look, up,3);
//                    mCamera->setPosition(pos);
//                    mCamera->lookAt(look);
//                    mCamera->setFixedYawAxis(true, up);


                //mSceneMgr->getEntity("Esfera")->setVisible(true);

                }
                  //else  mSceneMgr->getEntity("Esfera")->setVisible(false);
              }
          }
          frame ++;

//          IplImage* img = 0;
//          int i,j;
//          int altura,anchura,anchura_fila,canales;
//          uchar *data;
//
//        int x_v_cont = 0;
//        int y_v_cont = 0;
//
//        int x_v_total = 0;
//        int y_v_total = 0;
//        int visto=0;
//
//          img=_videoManager->getCurrentFrameIpl();
//          altura = img->height;
//          anchura = img->width;
//          anchura_fila = img->widthStep;
//          canales = img->nChannels;
//          data = (uchar *)img->imageData;
//          //printf("Procesando una imagen de %dx%d píxeles con %d canales\n", altura, anchura, canales);
//
//          for (i=0;i<anchura;i++){
//              for (j=0;j<altura;j++){ //verde oscuro
//                if ((data[i*anchura_fila+j*canales + 1] > 80) &&
//                !((data[i*anchura_fila+j*canales + 0] > data[i*anchura_fila+j*canales + 1]/2) ||
//                (data[i*anchura_fila+j*canales + 2] > data[i*anchura_fila+j*canales + 1]/2))){
////                data[i*anchura_fila+j*canales + 0]=255;
////                data[i*anchura_fila+j*canales + 1]=255;
////                data[i*anchura_fila+j*canales + 2]=255;
//                y_v_total = y_v_total + i;
//                x_v_total = x_v_total + j;
//                y_v_cont++;
//                x_v_cont++;
//                visto=1;
//                }
//              }
//          }
//          //cvShowImage("mainWin", img );
//if (visto==1){
//            int x_v_medio = x_v_total / x_v_cont;
//            int y_v_medio = y_v_total / y_v_cont;
//
//            printf("Punto medio verde en %d, %d, valor: %d\n",x_v_medio, y_v_medio, data[i*anchura_fila+j*canales+1]);
//}
//            visto = 0;
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
        Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
        Ogre::MaterialManager::getSingleton().setDefaultTextureFiltering(Ogre::TFO_TRILINEAR);

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

        Ogre::Entity* ent2 = mSceneMgr->createEntity("Esfera", "Esfera.mesh");
        Ogre::SceneNode* node2 = mSceneMgr->createSceneNode("Esfera");
        ent2->setMaterialName("Material2");
        ent2->setVisible(false);
        node2->attachObject(ent2);
        mSceneMgr->getRootSceneNode()->addChild(node2);

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

    float OgreWidget::getRotacion(int id){
        Ogre::Vector3 pos;  Ogre::Vector3 look;   Ogre::Vector3 up;
        _arDetector->getPosRot(pos, look, up,id);
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
            float phi= atan((v[1])/(v[0]));
            if(v[0]<0)
                phi= phi + PI;
            else if (v[1]<0)
                phi = phi + 2 * PI;
            return phi * 180 / PI;
    }
