#include "Scene.h"

#define PI 3.14159265

Scene::Scene(){
}

Scene::Scene(VideoManager *vm)
{
  _vmanager = vm;
  _vmanager->attach(this);
  _filter = Filter::Instance();
  //_Coordinator = new Coordinator(this);
  _grid_d=0;
  _center[0]=0.0;
  _center[1]=0.0;
  _fin[0]=0.0;
  _fin[1]=0.0;
  _lock = 0;
//  pthread_mutex_init(&ptmutex1,NULL);
  _objetos.clear();
//  _objs.clear();
  _marcas.clear();
//  _arobots.clear();

  //if((_marcas[4].getId()) < 0) return -1;

//  _arobots.push_back(ARobot(0));
//  _arobots.push_back(ARobot(1));
  //_robots[2] = Robot();

}


void Scene::init(){
  double p_center[2] = {0.0, 0.0};
  char p_patt[] = "data/4x4_26.patt";
  _marcas.push_back(new Marca(30.0,p_center,p_patt));
  _marcas[0]->setVisible(false);
  //if((_marcas[0].getId()) < 0) return -1;

  strcpy(p_patt,"data/4x4_78.patt");
  _marcas.push_back(new Marca(30.0,p_center,p_patt));
  _marcas[1]->setVisible(false);
  //if((_marcas[1].getId()) < 0) return -1;

  strcpy(p_patt,"data/simple.patt");
  _marcas.push_back(new Marca(50.0,p_center,p_patt));
  _marcas[2]->setVisible(false);
  //if((_marcas[2].getId()) < 0) return -1;

  strcpy(p_patt,"data/4x4_95.patt");
  _marcas.push_back(new Marca(50.0,p_center,p_patt));
  _marcas[3]->setVisible(false);
  //if((_marcas[3].getId()) < 0) return -1;

  strcpy(p_patt,"data/4x4_85.patt");
  _marcas.push_back(new Marca(50.0,p_center,p_patt));
  _marcas[4]->setVisible(false);
}

Marca* Scene::getMarca(int id){
    return _marcas[id];
}

vector<Objeto*> Scene::getObjetos(){
    return _objetos;
}

int ** Scene::getMap(){
    return _map;
}

//ARobot* Scene::getARobot(int id){
//    return &(_arobots[id]);
//}

int Scene::getPixel(int x, int y){
    return _map[static_cast<int>(x/_grid_d)][static_cast<int>(y/_grid_d)];
}

void Scene::attach(Coordinator *c){
    _Coordinator.push_back(c);
}

void Scene::setP_sup(double x,double y){_p_sup[0] = x; _p_sup[1] = y;}
void Scene::setP_sup_der(double x,double y){_p_sup_der[0] = x; _p_sup_der[1] = y;}
void Scene::setP_sup_izq(double x,double y){_p_sup_izq[0] = x; _p_sup_izq[1] = y;}
void Scene::setP_inf(double x,double y){_p_inf[0] = x; _p_inf[1] = y;}
void Scene::setP_inf_der(double x,double y){_p_inf_der[0] = x; _p_inf_der[1] = y;}
void Scene::setP_inf_izq(double x,double y){_p_inf_izq[0] = x; _p_inf_izq[1] = y;}
void Scene::setAlto(int alto){_alto = alto;}
void Scene::setAncho(int ancho){_ancho = ancho;}
double* Scene::getP_sup(){return _p_sup;}
double* Scene::getP_sup_der(){return _p_sup_der;}
double* Scene::getP_sup_izq(){return _p_sup_izq;}
double* Scene::getP_inf(){return _p_inf;}
double* Scene::getP_inf_der(){return _p_inf_der;}
double* Scene::getP_inf_izq(){return _p_inf_izq;}
int Scene::getGrid(){return _grid_d;}

void Scene::setARTK(ARTKDetector *artk){
    _arDetector = artk;
}

vector<Marca*> Scene::getMarcas(){
    return _marcas;
}

void Scene::crearMarca(Marca* m){
    cout<<"Scene:: Crear Marca"<<endl;
    for (int x=m->getMin()[0]/_grid_d; x<=m->getMax()[0]/_grid_d; x++){
        for (int y=m->getMin()[1]/_grid_d; y<=m->getMax()[1]/_grid_d; y++){
//            cout<<"Modifica mapa"<<endl;
            _map[x][y] = m->getId()-2;
        }
    }
    int au[2] = {((((m->getMax()[0])/_grid_d) - ((m->getMin()[0])/_grid_d) +1)/2),((((m->getMax()[1])/_grid_d) - ((m->getMin()[1])/_grid_d) +1)/2)};
    m->setRatio(au);
    _marcas[m->getId()] = m;
}
void Scene::modificarMarca(Marca* m){
    cout<<"Scene:: Modificar Marca"<<endl;
   // modificar marca en el map
     for (int x=0; x<=_ancho/_grid_d; x++){
         for (int y=0; y<=_alto/_grid_d; y++){
             _map[x][y] = -1;
         }
     }
     for (int x=m->getMin()[0]/_grid_d; x<=m->getMax()[0]/_grid_d; x++){
         for (int y=m->getMin()[1]/_grid_d; y<=m->getMax()[1]/_grid_d; y++){
             _map[x][y] = m->getId()-2;
         }
     }
    _marcas[m->getId()] = m;
}
void Scene::borrarMarca(Marca* m){
    cout<<"Scene:: Borrar Marca"<<endl;
    for (int x=_marcas[m->getId()]->getMin()[0]/_grid_d; x<=_marcas[m->getId()]->getMax()[0]/_grid_d; x++){
        for (int y=_marcas[m->getId()]->getMin()[1]/_grid_d; y<=_marcas[m->getId()]->getMax()[1]/_grid_d; y++){
            _map[x][y] = -1;
        }
    }
    _marcas[m->getId()] = m;
}
//void Scene::setMarcas(Marca* marcas[5]){
//    for (int i=0; i<5; i++){
//        if (i>1){
//            if (marcas[i]->getVisible()){
//              if(_marcas[i]->getVisible()){
//                  cout<<"Scene:: Modificar Marca:"<<i<<endl;
//                  if ((_marcas[i]->getMax()[0]/_grid_d != marcas[i]->getMax()[0]/_grid_d)||
//                      (_marcas[i]->getMax()[1]/_grid_d != marcas[i]->getMax()[1]/_grid_d)||
//                      (_marcas[i]->getMin()[0]/_grid_d != marcas[i]->getMin()[0]/_grid_d)||
//                      (_marcas[i]->getMin()[1]/_grid_d != marcas[i]->getMin()[1]/_grid_d)){
//                      // modificar marca en el map
//                        for (int x=_marcas[i]->getMin()[0]/_grid_d; x<=_marcas[i]->getMax()[0]/_grid_d; x++){
//                            for (int y=_marcas[i]->getMin()[1]/_grid_d; y<=_marcas[i]->getMax()[1]/_grid_d; y++){
//                                _map[x][y] = -1;
//                            }
//                        }
//                        for (int x=marcas[i]->getMin()[0]/_grid_d; x<=marcas[i]->getMax()[0]/_grid_d; x++){
//                            for (int y=marcas[i]->getMin()[1]/_grid_d; y<=marcas[i]->getMax()[1]/_grid_d; y++){
//                                _map[x][y] = i-2;
//                            }
//                        }
//                  }
//              }else {
//                cout<<"Scene:: Crear Marca:"<<i<<endl;
//                  // Crear marca en el map
//                for (int x=marcas[i]->getMin()[0]/_grid_d; x<=marcas[i]->getMax()[0]/_grid_d; x++){
//                    for (int y=marcas[i]->getMin()[1]/_grid_d; y<=marcas[i]->getMax()[1]/_grid_d; y++){
//                        _map[x][y] = i-2;
//                    }
//                }
//              }
//            } else {
//              if (_marcas[i]->getVisible()){
//                cout<<"Scene:: Borrar Marca:"<<i<<endl;
//                  // borrar marca en el map
//                for (int x=_marcas[i]->getMin()[0]/_grid_d; x<=_marcas[i]->getMax()[0]/_grid_d; x++){
//                    for (int y=_marcas[i]->getMin()[1]/_grid_d; y<=_marcas[i]->getMax()[1]/_grid_d; y++){
//                        _map[x][y] = -1;
//                    }
//                }
//              }//fin if
//            }//fin else
//        }
//        marcas[i]->setRatio(((marcas[i]->getMax()[0])/_grid_d) - ((marcas[i]->getMin()[0])/_grid_d));
//        _marcas[i] = marcas[i];
//    }
//}

void Scene::setMap(int h, int a, int valor){
    _map[h][a] = valor;
}

int Scene::getMap(int h, int a){
    return _map[h][a];
}

int Scene::getAncho(){
    return _ancho;
}

int Scene::getAlto(){
    return _alto;
}

void Scene::Actualizar(int accion, int id){
    ARMarkerInfo *mark1;
    ARMarkerInfo *mark2;
    if(id==0 && _center[0]==0.0){
         mark1 = getMarca(0)->getMarkerInfo();
        if (getRotacion(0)>90){
            setP_sup(mark1->vertex[2][0], mark1->vertex[2][1]);
            _filter->setPsup(mark1->vertex[2][0], mark1->vertex[2][1]);
            setP_sup_der(mark1->vertex[3][0], mark1->vertex[3][1]);
            setP_sup_izq(mark1->vertex[1][0], mark1->vertex[1][1]);
        }else{
            setP_sup(mark1->vertex[1][0], mark1->vertex[1][1]);
            _filter->setPsup(mark1->vertex[1][0], mark1->vertex[1][1]);
            setP_sup_der(mark1->vertex[2][0], mark1->vertex[2][1]);
            setP_sup_izq(mark1->vertex[0][0], mark1->vertex[0][1]);
        }
        _center[0] = mark1->pos[0];
        _center[1] = mark1->pos[1];
        std::cout<<"Scene:: Posicion inicio: "<<_center[0]<<":"<<_center[1]<<std::endl;
    }else  if(id==1 && _fin[0]==0.0 && _center[0]!=0.0){
        mark2 = getMarca(1)->getMarkerInfo();
        if(getRotacion(1)>90){
            setP_inf(mark2->vertex[0][0], mark2->vertex[0][1]);
            _filter->setPinf(mark2->vertex[0][0], mark2->vertex[0][1]);
            setP_inf_der(mark2->vertex[3][0], mark2->vertex[3][1]);
            setP_inf_izq(mark2->vertex[1][0], mark2->vertex[1][1]);
        }else{
            setP_inf(mark2->vertex[3][0], mark2->vertex[3][1]);
            _filter->setPinf(mark2->vertex[3][0], mark2->vertex[3][1]);
            setP_inf_der(mark2->vertex[2][0], mark2->vertex[2][1]);
            setP_inf_izq(mark2->vertex[0][0], mark2->vertex[0][1]);
        }
        _fin[0] = mark2->pos[0];
        _fin[1] = mark2->pos[1];

        _grid_d = 0;
        for (int j=0; j<2; j++){
            if (getMarca(j)->getVisible()){
                double m[2],m1[2];
                m[0] = getMarca(j)->getMarkerInfo()->vertex[0][0];
                m[1] = getMarca(j)->getMarkerInfo()->vertex[0][1];
                m1[0] = getMarca(j)->getMarkerInfo()->vertex[2][0];
                m1[1] = getMarca(j)->getMarkerInfo()->vertex[2][1];
                if(sqrt(pow(m1[0]-m[0],2)+pow(m1[1]-m[1],2)) > _grid_d){
                    _grid_d = sqrt(pow(m1[0]-m[0],2)+pow(m1[1]-m[1],2));
                }
            }
        }
        _map = new int *[_ancho/_grid_d + 1];
        for (int i = 0; i <= _ancho/_grid_d; i++){
            _map[i] = new int [_alto/_grid_d + 1];
            for (int j = 0; j <= _alto/_grid_d; j++){
                _map[i][j] = -1;
            }
        }
        cout<<"Scene:: alto: "<<_alto/_grid_d+1<<" ancho: "<<_ancho/_grid_d+1<<endl;
        _filter->setGrid(_grid_d);
        cout<<"Scene:: Posicion fin: "<<_fin[0]<<":"<<_fin[1]<<endl;
        cout<<"Scene:: Tamaño del grid: "<<_grid_d<<endl;
    }else if((id!=0 && id!=1) || (accion == 4 || accion ==5 || accion == 6)){
        for (vector<Coordinator*>::iterator it = _Coordinator.begin(); it != _Coordinator.end(); ++it) {
//            cout<<"Scene:: Actualizar "<<endl;
            (*it)->Actualizar(accion,id);
        }
    }
}

vector<int> Scene::arrayToVectorMap(){
        vector<int> ret;
        for(int i = 0; i <= _ancho/_grid_d; i++){
                for(int j = 0; j <= _alto/_grid_d; j++){
                        ret.push_back(_map[i][j]);
                }
        }
        return ret;
}

//void *Scene::Busca(void *arg){
//    return ((Scene *)arg)->Busca_Solucion(arg);
//}
//
//void *Scene::Busca_Solucion(void *arg){
//  int *ida = (int*)arg;
//  int id = ida[0];
//  int posx = ida[1];
//  int posy = ida[2];
//  int posxF = ida[3];
//  int posyF = ida[4];
//  State * e = new State(id,posx,posy,_ancho%_grid_d,_alto%_grid_d,posxF,posyF,arrayToVectorMap());
//  _path = false;
//  cout<<"posx: "<<posx<<endl;
//  cout<<"posx de State: "<<e->get_posx()<<endl;
//  try{
//      object mainobj = import("__main__");
//      object dictionary(mainobj.attr("__dict__"));
//      object result;
//      result = exec_file("AI.py",dictionary, dictionary);
//      object Search = dictionary["Ejecutar"];
//      if(!Search.is_none()){
//        boost::shared_ptr<State> State(e);
//          Search(ptr(State.get()));
//          State *obj = ptr(State.get());
//          std::vector<char> aux = obj->get_mov();
//          for(std::vector<char>::iterator it = aux.begin(); it != aux.end(); it++)
//              _lMov.push_back(*it);
//          pthread_mutex_lock(&ptmutex1);
//          _path = true;
//          cout<<"valor de path"<<_path<<endl;
//          pthread_mutex_unlock(&ptmutex1);
//      }
//  }catch(boost::python::error_already_set const &){}
//  cout<<"valor de path"<<_path<<endl;
//  return NULL;
//}

float Scene::getAngulo(float v[2]){
    v[1] = -v[1];
    float mod = atan((v[1])/v[0]);
    if (v[0]<0) mod = mod + PI;
    else if ((v[1])<0) mod = mod + 2 * PI;
    return mod * 180 / PI;
}

float Scene::getRotacion(int id){
    Ogre::Vector3 pos;  Ogre::Vector3 look;   Ogre::Vector3 up;
    Marca *maux = getMarca(id);
    _arDetector->getPosRot(pos, look, up,maux);
    float mod = atan((-up[0])/up[2]) - PI;
    if (up[2]<0) mod = mod + PI;
    else if ((-up[0])<0) mod = mod + 2 * PI;
    if (mod<0) mod = mod + 2 * PI;
    return mod * 180 / PI;
}

double* Scene::getCenter(){
    return _center;
}

double* Scene::getFin(){
    return _fin;
}

ARTKDetector* Scene::getARTK(){
    return _arDetector;
}


//int Scene::getLock(){
//    return _lock;
//}
//void Scene::setLock(int id){
//    _lock = id;
//}
//
//int Scene::getObjeto(int id){
//    int obj = -1;
//    for (uint i=0; i < _objetos.size(); i++){
//        if (_objetos[i].getRobot() == id){
//            obj = i;
//            break;
//        }
//    }
//    return obj;
//}

//int Scene::getPrioritario(){
//    int obj = 0;
//    int prioridad = -1;
//    for (uint i=0; i < _objetos.size(); i++){
//        if (_objetos[i].getPrioridad() > prioridad){
//            obj = i;
//            prioridad = _objetos[obj].getPrioridad();
//        }
//    }
//    return obj;
//}

void Scene::addObject(Objeto* ob){
    cout<<"AddObject"<<endl;
    _objetos.push_back(ob);
    double* pmax = ob->getMax();
    double* pmin = ob->getMin();
    for (int x=pmin[0]/_grid_d; x<=pmax[0]/_grid_d; x++){
        for (int y=pmin[1]/_grid_d; y<=pmax[1]/_grid_d; y++){
            _map[x][y] = ob->getId() + 3;
        }
    }
    Actualizar(4,ob->getId());
}

void Scene::deleteObject(int id){
    for (uint i=0; i < _objetos.size(); i++){
        if (_objetos[i]->getId() == id){
            cout<<"Scene:: Borrar obj"<<endl;
            double* pmax = _objetos[i]->getMax();
            double* pmin = _objetos[i]->getMin();
            for (int x=pmin[0]/_grid_d; x<=pmax[0]/_grid_d; x++){
                for (int y=pmin[1]/_grid_d; y<=pmax[1]/_grid_d; y++){
                    _map[x][y] = -1;
                }
            }
            _objetos.erase(_objetos.begin() + i);
        }
    }
    Actualizar(6,id);
}

void Scene::changeObject(int id, double fin[2], double pM[2], double pm[2]){
    for (uint i=0; i < _objetos.size(); i++){
        if (_objetos[i]->getId() == id){
            cout<<"Scene:: Cambia antiguo"<<endl;
            double* pmax = _objetos[i]->getMax();
            double* pmin = _objetos[i]->getMin();
            for (int x=pmin[0]/_grid_d; x<=pmax[0]/_grid_d; x++){
                for (int y=pmin[1]/_grid_d; y<=pmax[1]/_grid_d; y++){
                    _map[x][y] = -1;
                }
            }
//            double* point = _objetos[i].getPos();
//            _map[(int)(point[0]/_grid_d)][(int)(point[1]/_grid_d)] = 0;
            for (int x=pm[0]/_grid_d; x<=pM[0]/_grid_d; x++){
                for (int y=pm[1]/_grid_d; y<=pM[1]/_grid_d; y++){
                    _map[x][y] = _objetos[i]->getId() + 3;
                }
            }
//            _objetos[i].setPos(fin, pM, pm);
//            _map[(int)(fin[0]/_grid_d)][(int)(fin[1]/_grid_d)] = _objetos[i].getId();
        }
    }
    Actualizar(5,id);
}

//////void Scene::actualizaColores(){
////////    bool nuevo = false;
//////    for (uint i=0; i < _objs.size(); i++){
//////        int p = pertenece(_objs[i]);
//////        if (p >= 0){
//////            _objs[i] = _objetos[p];
////////        }else{
////////            nuevo = true;
//////        }
//////    }
//////    _objetos.clear();
//////    for (uint i=0; i < _objs.size(); i++){
//////        _objetos.push_back(_objs[i]);
//////    }
//////    _objs.clear();
//////}
//////
//void Scene::clearColors(){
//    _objetos.clear();
//}
//////
//////void Scene::clearObjs(){
//////    _objs.clear();
//////}

int Scene::pertenece(Objeto *ob){
    int pertenece = -1;
    for (uint i=0; i < _objetos.size(); i++){
        if (_objetos[i] == ob){
            pertenece = i;
            break;
        }
    }
    return pertenece;
}

Scene& Scene::operator=(const Scene &s){
  for (int i=0;i<5;i++)
    this->_marcas[i] = s._marcas[i];
//  for (int j=0;j<3;j++)
//    this->_arobots[j] = s._arobots[j];
  return *this;
}
