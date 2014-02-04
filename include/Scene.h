#ifndef SCENE_H
 #define SCENE_H
#include "Marca.h"
#include "Robot.h"
#include <vector>
using namespace std;

class Scene
{
   public:
      static Scene* Instance();
      Marca* getMarca(int id);
      Robot* getRobot(int id);
      void setP_sup(double x,double y);
      void setP_sup_der(double x,double y);
      void setP_sup_izq(double x,double y);
      void setP_inf(double x,double y);
      void setP_inf_der(double x,double y);
      void setP_inf_izq(double x,double y);
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
      static Scene* pinstance;
      vector<Robot> _robots;
      vector<Marca> _marcas;
      double _p_sup[2];
      double _p_sup_der[2];
      double _p_inf_der[2];
      double _p_inf[2];
      double _p_inf_izq[2];
      double _p_sup_izq[2];
};
#endif
