#include "ogrewindow.h"

    OgreWindow::OgreWindow() :
        mOgreWidget(),
        mExited(false)
        //mRobot()
    {
      set_border_width(1);

      Gtk::VBox *vb = new Gtk::VBox(false,0);

      Gtk::Button *mb = new Gtk::Button("Avanzar");
      mb->signal_clicked().connect(sigc::mem_fun(*this,&OgreWindow::avanzar));
      Gtk::Button *mb2 = new Gtk::Button("Retroceder");
      mb2->signal_clicked().connect(sigc::mem_fun(*this,&OgreWindow::retroceder));
      Gtk::Button *mb3 = new Gtk::Button("Giro derecha");
      mb3->signal_clicked().connect(sigc::mem_fun(*this,&OgreWindow::derecha));
      Gtk::Button *mb4 = new Gtk::Button("Giro izquierda");
      mb4->signal_clicked().connect(sigc::mem_fun(*this,&OgreWindow::izquierda));
      vb->pack_start(*mb,true,true,0);
      vb->pack_start(*mb2,true,true,0);
      vb->pack_start(*mb3,true,true,0);
      vb->pack_start(*mb4,true,true,0);
      vb->pack_start(mOgreWidget,true,true,10);

      add(*vb);
      show_all();
    }

    void OgreWindow::avanzar()
    {
//      mRobot.avanzar();
    }

    void OgreWindow::retroceder()
    {
//      mRobot.retroceder();

    }

    void OgreWindow::derecha()
    {
//      mRobot.derecha();
    }

    void OgreWindow::izquierda()
    {
//      mRobot.izquierda();
    }

    OgreWindow::~OgreWindow()
    {
    }

    bool OgreWindow::on_delete_event(GdkEventAny* event)
    {
        mExited = true;
        return false;
    }

    bool OgreWindow::on_key_press_event(GdkEventKey *event) {
       std::cout << "keydown\n";
//       mRobot.parar();
       return true;
    }

    bool OgreWindow::on_key_release_event(GdkEventKey *event) {
       std::cout << "keyup\n";
       return true;
    }
