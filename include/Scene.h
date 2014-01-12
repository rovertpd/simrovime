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
   protected:
      Scene();
      //Scene(const Scene & ) ;
      Scene &operator= (const Scene &s) ;
   private:
      static Scene* pinstance;
      vector<Robot> _robots;
      vector<Marca> _marcas;
};
#endif
