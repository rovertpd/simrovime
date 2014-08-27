#ifndef COORD_H
 #define COORD_H
#include <OGRE/Ogre.h>
#include <boost/lexical_cast.hpp>
#include <boost/python/str.hpp>
#include <vector>

class Coordinator;

#include "ARobot.h"
#include "Scene.h"
#include "ARTKDetector.h"
#include "Marca.h"
#include "Objeto.h"
#include "State.h"

#define PI 3.14159265

using namespace std;

class Coordinator
{
   public:
      Coordinator();
      Coordinator(Scene* scn);
      void init();
      ARobot* getARobot(int id);
      void Actualizar(int event, int id);
      void setMap(int h, int a, int valor);
      int getMap(int h, int a);
      int getObjeto(int id);
      int getPrioritario();
      int anyIdle();
      int havePriority(Objeto* ob);
      int getColision(int id);

   protected:
      //Scene(const Scene & ) ;
      vector<int> arrayToVectorMap();
      void *Busca_Solucion(void *arg);
      static void *Busca(void *arg);
   private:
      Scene *_scene;
      int _lock;
      vector<ARobot*> _arobots;
      int **_map;
      bool _aRo[3];
};
#endif
