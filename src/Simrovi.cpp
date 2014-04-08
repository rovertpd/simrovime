
#include <gtkmm/main.h>
    #include "ogrewindow.h"
    #include "Scene.h"


BOOST_PYTHON_MODULE( State )
{
        class_<Estado>("Estado", init<int,int,int,int,int,int,int,std::vector<int> >())
                .def("get_id", &Estado::get_id)
                .def("add_mov", &Estado::add_mov)
                .def("get_posx", &Estado::get_posx)
                .def("get_posy", &Estado::get_posy)
                .def("get_gridx", &Estado::get_gridx)
                .def("get_gridy", &Estado::get_gridy)
                .def("get_posxE", &Estado::get_posxE)
                .def("get_posyE", &Estado::get_posyE)
                .def("get_mapa", &Estado::get_mapa)
                ;
}


    int main (int argc, char *argv[])
    {
        Py_Initialize();
        initState();

        Ogre::Root* root = new Ogre::Root();

        if (!root->showConfigDialog())
            return -1;

        root->initialise(false);

        Gtk::Main kit(argc, argv);

        Scene::Instance();

        OgreWindow oWindow;
        oWindow.show();

        while (!oWindow.hasExited())
        {
            kit.iteration();
            // you could also call renderOneFrame() here instead.
        }

        delete root;
        Py_Finalize();
        return 0;
    }
