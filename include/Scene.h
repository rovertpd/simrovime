#ifndef SCENE_H
 #define SCENE_H
#include "Marca.h"
#include "Robot.h"
#include "Objeto.h"
#include "ARTKDetector.h"
#include <Ogre.h>
#include <vector>
using namespace std;

class Scene
{
   public:
      static Scene* Instance();
      void getMarcas(Marca marcas[5]);
      void setMarcas(Marca marcas[5]);
      Marca* getMarca(int id);
      Robot* getRobot(int id);
      void setP_sup(double x,double y);
      void setP_sup_der(double x,double y);
      void setP_sup_izq(double x,double y);
      void setP_inf(double x,double y);
      void setP_inf_der(double x,double y);
      void setP_inf_izq(double x,double y);
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
      int pertenece(Objeto ob);
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
   private:
      ARTKDetector* _arDetector;
      static Scene* pinstance;
      double _center[2];
      double _fin[2];
      int _lock;
      vector<Robot> _robots;
      vector<Marca> _marcas;
      vector<Objeto> _objetos;
      vector<Objeto> _objs;
      double _p_sup[2];
      double _p_sup_der[2];
      double _p_inf_der[2];
      double _p_inf[2];
      double _p_inf_izq[2];
      double _p_sup_izq[2];
};
#endif
