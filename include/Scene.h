#ifndef SCENE_H
 #define SCENE_H
#include <OGRE/Ogre.h>
#include <boost/lexical_cast.hpp>
#include <boost/python/str.hpp>
#include <vector>
#include "Objeto.h"

class Scene;

#include "ARTKDetector.h"
#include "VideoManager.h"
#include "Coordinador.h"
#include "ARobot.h"
#include "Marca.h"
#include "Filter.h"
#include "estado.h"

using namespace std;

class Scene
{
   public:
      Scene();
      Scene(VideoManager *vm);

      void attach(Coordinador *c);

      vector<Marca*> getMarcas();
//      void setMarcas(Marca* marcas[5]);
      Marca* getMarca(int id);
      vector<Objeto*> getObjetos();
//      ARobot* getARobot(int id);
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
      ARTKDetector* getARTK();
      void Actualizar(int accion, int id);
      float getAngulo(float v[2]);
      float getRotacion(int id);
//      int getLock();
//      int getObjeto(int id);
//      int getPrioritario();
//      void setLock(int id);
      double * getCenter();
      double * getFin();
      void addObject(Objeto* ob);
      void deleteObject(int id);
      void changeObject(int id, double fin[2], double pM[2], double pm[2]);
//      void actualizaColores();
//      void clearColors();
//      void clearObjs();
      void setMap(int h, int a, int valor);
      int getMap(int h, int a);
      int pertenece(Objeto* ob);
      int getGrid();
      double* getP_sup();
      double* getP_sup_der();
      double* getP_sup_izq();
      double* getP_inf();
      double* getP_inf_der();
      double* getP_inf_izq();
      vector<int> arrayToVectorMap();
      void crearMarca(Marca* m);
      void modificarMarca(Marca* m);
      void borrarMarca(Marca* m);
   protected:
      //Scene(const Scene & ) ;
      Scene &operator= (const Scene &s) ;
      //vector<int> arrayToVectorMap();
      //void *Busca_Solucion(void *arg);
      //static void *Busca(void *arg);
   private:
      pthread_mutex_t ptmutex1;
      pthread_t mithread001;
      ARTKDetector* _arDetector;
      static Scene* pinstance;
      double _center[2];
      double _fin[2];
      int _lock;
      VideoManager* _vmanager;
      vector<Marca*> _marcas;
      vector<Objeto*> _objetos;
      //vector<Objeto> _objs;
      int _grid_d;
      int _alto;
      int _ancho;
      int **_map;
      vector<Coordinador*> _coordinador;
      double _p_sup[2];
      double _p_sup_der[2];
      double _p_inf_der[2];
      double _p_inf[2];
      double _p_inf_izq[2];
      double _p_sup_izq[2];
      Filter* _filter;
};
#endif
