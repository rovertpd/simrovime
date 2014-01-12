#include "Scene.h"


Scene* Scene::pinstance = 0;// Inicializar el puntero
Scene* Scene::Instance ()
{
  if (pinstance == 0)  // ¿Es la primera llamada?
  {
    pinstance = new Scene; // Creamos la instancia
  }
  return pinstance; // Retornamos la dirección de la instancia
}

Marca* Scene::getMarca(int id){
    return &(_marcas[id]);
}

Robot* Scene::getRobot(int id){
    return &(_robots[id]);
}

Scene::Scene()
{
  double p_center[2] = {0.0, 0.0};
  char p_patt[] = "data/4x4_95.patt";
  _marcas.push_back(Marca(30.0,p_center,p_patt));
  _marcas[0].setVisible(false);
  //if((_marcas[0].getId()) < 0) return -1;

  strcpy(p_patt,"data/identic.patt");
  _marcas.push_back(Marca(120.0,p_center,p_patt));
  _marcas[1].setVisible(false);
  //if((_marcas[1].getId()) < 0) return -1;

  strcpy(p_patt,"data/4x4_3.patt");
  _marcas.push_back(Marca(30.0,p_center,p_patt));
  _marcas[2].setVisible(false);
  //if((_marcas[2].getId()) < 0) return -1;

  strcpy(p_patt,"data/simple.patt");
  _marcas.push_back(Marca(50.0,p_center,p_patt));
  _marcas[3].setVisible(false);
  //if((_marcas[3].getId()) < 0) return -1;

  strcpy(p_patt,"data/simple.patt");
  _marcas.push_back(Marca(50.0,p_center,p_patt));
  _marcas[4].setVisible(false);
  //if((_marcas[4].getId()) < 0) return -1;

  _robots.push_back(Robot());
  //_robots[1] = Robot();
  //_robots[2] = Robot();

}

Scene& Scene::operator=(const Scene &s){
  for (int i=0;i<5;i++)
    this->_marcas[i] = s._marcas[i];
  for (int j=0;j<3;j++)
    this->_robots[j] = s._robots[j];
  return *this;
}
