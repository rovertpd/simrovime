#ifndef SCENE_H
 #define SCENE_H
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

using namespace std;

class Scene
{
   public:
      static Scene* Instance();
      void getMarcas(Marca marcas[5]);
      void setMarcas(Marca marcas[5]);
      Marca* getMarca(int id);
      ARobot* getARobot(int id);
      void setP_sup(double x,double y);
      void setP_sup_der(double x,double y);
      void setP_sup_izq(double x,double y);
      void setP_inf(double x,double y);
      void setP_inf_der(double x,double y);
      void setP_inf_izq(double x,double y);
      void setAlto(int alto);
      void setAncho(int ancho);
      int getAncho();
      int getAlto();
      void setARTK(ARTKDetector *artk);
      void Actualizar();
      float getAngulo(float v[2]);
      float getRotacion(int id);
      int getLock();
      int getObjeto(int id);
      int getPrioritario();
      void setLock(int id);
      void addObject(Objeto ob);
      void actualizaColores();
      void clearColors();
      void clearObjs();
      void setMap(int h, int a, int valor);
      int getMap(int h, int a);
      int pertenece(Objeto ob);
      int getGrid();
      double* getP_sup();
      double* getP_sup_der();
      double* getP_sup_izq();
      double* getP_inf();
      double* getP_inf_der();
      double* getP_inf_izq();
   protected:
      Scene();
      //Scene(const Scene & ) ;
      Scene &operator= (const Scene &s) ;
      vector<int> arrayToVectorMap();
      void *Busca_Solucion(void *arg);
      static void *Busca(void *arg);
   private:
      pthread_mutex_t ptmutex1;
      pthread_t mithread001;
      ARTKDetector* _arDetector;
      static Scene* pinstance;
      double _center[2];
      double _fin[2];
      int _lock;
      vector<ARobot> _arobots;
      vector<Marca> _marcas;
      vector<Objeto> _objetos;
      vector<Objeto> _objs;
      int _grid_d;
      int _alto;
      int _ancho;
      int **_map;
};
#endif
