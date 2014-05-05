#ifndef COORD_H
 #define COORD_H
#include "Marca.h"
#include "ARobot.h"
#include "Objeto.h"
#include "ARTKDetector.h"
#include <OGRE/Ogre.h>
#include <boost/lexical_cast.hpp>
#include <boost/python/str.hpp>
#include "estado.h"
#include <vector>
#include "ia.h"

class Coordinador;

#include "Scene.h"


#define PI 3.14159265

using namespace std;

class Coordinador
{
   public:
      Coordinador();
      Coordinador(Scene* scn);
      ARobot* getARobot(int id);
      void Actualizar();
      int getLock();
      void setLock(int id);
      void setMap(int h, int a, int valor);
      int getMap(int h, int a);
   protected:
      //Scene(const Scene & ) ;
      vector<int> arrayToVectorMap();
      void *Busca_Solucion(void *arg);
      static void *Busca(void *arg);
   private:
      Scene *_scene;
      int _lock;
      vector<ARobot> _arobots;
      int **_map;
};
#endif
