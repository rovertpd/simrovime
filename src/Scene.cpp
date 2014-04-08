#include "Scene.h"

#define PI 3.14159265

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

ARobot* Scene::getARobot(int id){
    return &(_arobots[id]);
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

void Scene::getMarcas(Marca marcas[5]){
    for (int i=0; i<5; i++){
        marcas[i] = _marcas[i];
    }
}

void Scene::setMarcas(Marca marcas[5]){
    for (int i=0; i<5; i++){
        _marcas[i] = marcas[i];
    }
}

void Scene::Actualizar(){
    cout<<"Aqui 77"<<endl;
    ARMarkerInfo *mark1;
    ARMarkerInfo *mark2;
    if(getMarca(0)->getVisible() && _center[0]==0.0){
         mark1 = getMarca(0)->getMarkerInfo();
        if (getRotacion(0)>90){
            setP_sup(mark1->vertex[2][0], mark1->vertex[2][1]);
            setP_sup_der(mark1->vertex[3][0], mark1->vertex[3][1]);
            setP_sup_izq(mark1->vertex[1][0], mark1->vertex[1][1]);
        }else{
            setP_sup(mark1->vertex[1][0], mark1->vertex[1][1]);
            setP_sup_der(mark1->vertex[2][0], mark1->vertex[2][1]);
            setP_sup_izq(mark1->vertex[0][0], mark1->vertex[0][1]);
        }
        _center[0] = mark1->pos[0];
        _center[1] = mark1->pos[1];
        std::cout<<"Rotación inicio: "<<getRotacion(0)<<std::endl;
    }
    if(getMarca(1)->getVisible() && _fin[0]==0.0 && _center[0]!=0.0){
        mark2 = getMarca(1)->getMarkerInfo();
        if(getRotacion(1)>90){
            setP_inf(mark2->vertex[0][0], mark2->vertex[0][1]);
            setP_inf_der(mark2->vertex[3][0], mark2->vertex[3][1]);
            setP_inf_izq(mark2->vertex[1][0], mark2->vertex[1][1]);
        }else{
            setP_inf(mark2->vertex[3][0], mark2->vertex[3][1]);
            setP_inf_der(mark2->vertex[2][0], mark2->vertex[2][1]);
            setP_inf_izq(mark2->vertex[0][0], mark2->vertex[0][1]);
        }
        _fin[0] = mark2->pos[0];
        _fin[1] = mark2->pos[1];
        std::cout<<"Rotación fin: "<<getRotacion(1)<<std::endl;
    }

    ARobot* arobot;
    if (_fin[0] != 0.0){
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

        for (int i=2; i<5; i++){
            if (getMarca(i)->getVisible()&&(getLock()==0 || getLock()==i)){
                arobot = getARobot(i-2);
//                cout<<"Aqui 66"<<_path<<endl;
//                if (_path){
//                    cout<<"Aqui 11"<<endl;
//                    _path1 =false;
//                    _path = false;
//                    for(std::vector<char>::iterator it = _lMov.begin(); it != _lMov.end(); it++)
//                        cout<<"Accion: "<<*it<<endl;
//                        cout<<"Aqui 22"<<endl;
//                }else if (!_path1){
//                    cout<<"Aqui 33"<<endl;
//                    int arg[] = {i-2, getMarca(i)->getMarkerInfo()->pos[0], getMarca(i)->getMarkerInfo()->pos[1], _fin[0], _fin[1]};
//                    cout<<"XXXXXXX: "<<arg[1]<<endl;
//                    cout<<"Aqui 44"<<endl;
//                    if (pthread_create( &mithread001, NULL, &Scene::Busca,(void *)arg ) ) {
//                        printf("Error creando el hilo.");
//                        abort();
//                    }
//                    _path1 = true;
//                }
                double fin[2];
                arobot->getFin(fin);
                cout<<"Posicion finalLLLL: ["<<fin[0]<<","<<fin[1]<<"]"<<endl;
                if (fin[0] == -1.0){ //Inicializamos las posiciones de la trayectoria (los puntos finales)
                    arobot->planifica(getMarca(i)->getMarkerInfo(),_center,_fin,0,getRotacion(i));
                      cout<<"Inicializado"<<endl;
//                    double fin[2];
//                    if (getMarca(i)->getMarkerInfo()->pos[0] > ((_fin[0] - _center[0]) / 2 + _center[0])){
//                        if (getMarca(i)->getMarkerInfo()->pos[1] > ((_fin[1] - _center[1]) / 2 + _center[1])){
//                            robot->setDir(1);
//                            fin[0] = _fin[0];
//                            fin[1] = _fin[1];
//                            robot->setFin(fin);
//                        }else{
//                            robot->setDir(2);
//                            fin[0] = _fin[0];
//                            fin[1] = _center[1];
//                            robot->setFin(fin);
//                        }
//                    }else{
//                        if (getMarca(i)->getMarkerInfo()->pos[1] > ((_fin[1] - _center[1]) / 2 + _center[1])){
//                            robot->setDir(4);
//                            fin[0] = _center[0];
//                            fin[1] = _fin[1];
//                            robot->setFin(fin);
//                        }else{
//                            robot->setDir(3);
//                            fin[0] = _center[0];
//                            fin[1] = _center[1];
//                            robot->setFin(fin);
//                        }
//                    }
                }
                std::cout<<"Robot: "<<i<<" "<< "getLock: "<<getLock()<<std::endl;
                setLock(i);
                if (_objetos.size() > 0){
                    std::cout<<"Hay objetos de color: "<<_objetos.size()<<std::endl;
                    int nObjeto = getObjeto(i);  //Compruebo si el robot i tiene algún objeto asignado y devuelvo su posición
                    std::cout<<"Hay objetos asignado: "<<getObjeto(i)<<std::endl;
                    if (nObjeto >= 0){
                        std::cout<<"Tengo un objeto asignado: "<<nObjeto<<std::endl;
                        double posicionO[2];
                        _objetos[nObjeto].getPos(posicionO);

                        arobot->setFin(posicionO);
                        //cout<<"Objeto x: "<<posicionO[0]<<" Objeto y: "<<posicionO[1]<<endl;
                        double m[2];
                        float v[2];
                        m[0] = getMarca(i)->getMarkerInfo()->pos[0];
                        m[1] = getMarca(i)->getMarkerInfo()->pos[1];
                        v[0] = posicionO[0] - (getMarca(i)->getMarkerInfo()->pos[0]);
                        v[1] = posicionO[1] - (getMarca(i)->getMarkerInfo()->pos[1]);
                        float angulo = getAngulo(v);
                        arobot->setAng(angulo);
                        cout<<"Angulo "<<angulo<<endl;
                        cout<<"Rotacion "<<getRotacion(i)<<endl;
                        double dist01 = sqrt(pow((posicionO[0]-m[0]),2)+pow((posicionO[1]-m[1]),2));
                        if (dist01 > 75){
                            arobot->planifica(getMarca(i)->getMarkerInfo(),_center,_fin,3,getRotacion(i));
//                            if (getRotacion(i)>(angulo+10) || getRotacion(i)<(angulo-10)){
//                                if (angulo>getRotacion(i)){
//                                    if (angulo-getRotacion(i)<180){
//                                        robot->izquierda();
//                                    }
//                                    else{
//                                        robot->derecha();
//                                    }
//                                }else{
//                                    if (angulo-getRotacion(i)<180){
//                                        robot->derecha();
//                                    }
//                                    else{
//                                        robot->izquierda();
//                                    }
//                                }
//                                robot->setEst(6);
//                            }else{  //Ya está orientado (Dentro del umbral)
//                                robot->avanzar();
//                                robot->setEst(5);
//                            }
                        }else{  //Cerca del objeto (Distancia < limite)
                            setLock(0);
                            arobot->detener();
                        }
                    }else{  //No hay color asignado
                        std::cout<<"No tengo objeto asignado"<<std::endl;
                        if(_objetos.size() >= (i-2)){
                            std::cout<<"Hay suficientes objetos de color "<<std::endl;
                            int prioritario = getPrioritario();
                            _objetos[prioritario].setRobot(i);
                            double pO[2];
                            _objetos[prioritario].getPos(pO);
                            arobot->setFin(pO);
                            double m[2];
                            float v[2];
                            m[0] = getMarca(i)->getMarkerInfo()->pos[0];
                            m[1] = getMarca(i)->getMarkerInfo()->pos[1];
                            v[0] = pO[0] - (getMarca(i)->getMarkerInfo()->pos[0]);
                            v[1] = pO[1] - (getMarca(i)->getMarkerInfo()->pos[1]);
                            float angulo = getAngulo(v);
                            arobot->setAng(angulo);
                            double dist01 = sqrt(pow((pO[0]-m[0]),2)+pow((pO[1]-m[1]),2));
                            if (dist01 > 75){
                                arobot->planifica(getMarca(i)->getMarkerInfo(),_center,_fin,2,getRotacion(i));
//                                if (getRotacion(i)>(angulo+10) || getRotacion(i)<(angulo-10)){
//                                    if (angulo>getRotacion(i)){
//                                        if (angulo-getRotacion(i)<180){
//                                            robot->izquierda();
//                                        }
//                                        else{
//                                            robot->derecha();
//                                        }
//                                    }else{
//                                        if (angulo-getRotacion(i)<180){
//                                            robot->derecha();
//                                        }
//                                        else{
//                                            robot->izquierda();
//                                        }
//                                    }
//                                    robot->setEst(6);
//                                }else{  //Ya está orientado (Dentro del umbral)
//                                    robot->avanzar();
//                                    robot->setEst(5);
//                                }
                            }else{  //Cerca del objeto (Distancia < limite)
                                setLock(0);
                                arobot->detener();
                            }
                        }
                    }
                }else{
                    std::cout<<"Robot: "<<i<<" "<< " no tiene objeto a vigilar"<<std::endl;
                    double fin[2];
                    setLock(0);
                    if (arobot->getEst()>4){
                        arobot->planifica(getMarca(i)->getMarkerInfo(),_center,_fin,1,getRotacion(i));
//                        if (getMarca(i)->getMarkerInfo()->pos[0] > ((_fin[0] - _center[0]) / 2 + _center[0])){
//                            if (getMarca(i)->getMarkerInfo()->pos[1] > ((_fin[1] - _center[1]) / 2 + _center[1])){
//                                robot->setDir(1);
//                                fin[0] = _fin[0];
//                                fin[1] = _fin[1];
//                                robot->setFin(fin);
//                            }else{
//                                robot->setDir(2);
//                                fin[0] = _fin[0];
//                                fin[1] = _center[1];
//                                robot->setFin(fin);
//                            }
//                        }else{
//                            if (getMarca(i)->getMarkerInfo()->pos[1] > ((_fin[1] - _center[1]) / 2 + _center[1])){
//                                robot->setDir(4);
//                                fin[0] = _center[0];
//                                fin[1] = _fin[1];
//                                robot->setFin(fin);
//                            }else{
//                                robot->setDir(3);
//                                fin[0] = _center[0];
//                                fin[1] = _center[1];
//                                robot->setFin(fin);
//                            }
//                        }
                        arobot->setEst(0);
                    }else{
                        double posicionO[2];
                        arobot->getFin(posicionO);
                        double m[2];
                        float v[2];
                        m[0] = getMarca(i)->getMarkerInfo()->pos[0];
                        m[1] = getMarca(i)->getMarkerInfo()->pos[1];
                        v[0] = posicionO[0] - (getMarca(i)->getMarkerInfo()->pos[0]);
                        v[1] = posicionO[1] - (getMarca(i)->getMarkerInfo()->pos[1]);
                        float angulo = getAngulo(v);
                        arobot->setAng(angulo);
                        double dist01 = sqrt(pow((posicionO[0]-m[0]),2)+pow((posicionO[1]-m[1]),2));
                        if (dist01 > 75){
                            arobot->planifica(getMarca(i)->getMarkerInfo(),_center,_fin,4,getRotacion(i));
//                            if (getRotacion(i)>(angulo+10) || getRotacion(i)<(angulo-10)){
//                                if (angulo>getRotacion(i)){
//                                    if (angulo-getRotacion(i)<180){
//                                        robot->izquierda();
//                                    }
//                                    else{
//                                        robot->derecha();
//                                    }
//                                }else{
//                                    if (angulo-getRotacion(i)<180){
//                                        robot->derecha();
//                                    }
//                                    else{
//                                        robot->izquierda();
//                                    }
//                                }
//                                robot->setEst(2);
//                            }else{  //Ya está orientado (Dentro del umbral)
//                                robot->avanzar();
//                                robot->setEst(1);
//                            }
                        }else{
                            switch(arobot->getDir()){
                                case 4:
                                    arobot->setDir(1);
                                    fin[0] = _fin[0];
                                    fin[1] = _fin[1];
                                    arobot->setFin(fin);
                                break;
                                case 3:
                                    arobot->setDir(4);
                                    fin[0] = _center[0];
                                    fin[1] = _fin[1];
                                    arobot->setFin(fin);
                                break;
                                case 2:
                                    arobot->setDir(3);
                                    fin[0] = _center[0];
                                    fin[1] = _center[1];
                                    arobot->setFin(fin);
                                break;
                                case 1:
                                    arobot->setDir(2);
                                    fin[0] = _fin[0];
                                    fin[1] = _center[1];
                                    arobot->setFin(fin);
                                break;
                            }
                        }
                    }
                }
//                double m[4],m1[4];
//                m[0] = getMarca(i)->getMarkerInfo()->vertex[0][0];
//                m1[0] = getMarca(i)->getMarkerInfo()->vertex[0][1];
//                m[1] = getMarca(i)->getMarkerInfo()->vertex[1][0];
//                m1[1] = getMarca(i)->getMarkerInfo()->vertex[1][1];
//                m[2] = getMarca(i)->getMarkerInfo()->vertex[2][0];
//                m1[2] = getMarca(i)->getMarkerInfo()->vertex[2][1];
//                m[3] = getMarca(i)->getMarkerInfo()->vertex[3][0];
//                m1[3] = getMarca(i)->getMarkerInfo()->vertex[3][1];
            }
        }
    }
}

vector<int> Scene::arrayToVectorMap(){
        vector<int> ret;
        for(int i = 0; i < _ancho%_grid_d; i++)
                for(int j = 0; j < _alto%_grid_d; j++)
                        ret.push_back(_map[i][j]);
        return ret;
}

void *Scene::Busca(void *arg){
    return ((Scene *)arg)->Busca_Solucion(arg);
}

void *Scene::Busca_Solucion(void *arg){
  int *ida = (int*)arg;
  int id = ida[0];
  int posx = ida[1];
  int posy = ida[2];
  int posxF = ida[3];
  int posyF = ida[4];
  Estado * e = new Estado(id,posx,posy,_ancho%_grid_d,_alto%_grid_d,posxF,posyF,arrayToVectorMap());
  _path = false;
  cout<<"posx: "<<posx<<endl;
  cout<<"posx de estado: "<<e->get_posx()<<endl;
  try{
      object mainobj = import("__main__");
      object dictionary(mainobj.attr("__dict__"));
      object result;
      result = exec_file("IA.py",dictionary, dictionary);
      object busqueda = dictionary["Ejecutar"];
      if(!busqueda.is_none()){
        boost::shared_ptr<Estado> estado(e);
          busqueda(ptr(estado.get()));
          Estado *obj = ptr(estado.get());
          std::vector<char> aux = obj->get_mov();
          for(std::vector<char>::iterator it = aux.begin(); it != aux.end(); it++)
              _lMov.push_back(*it);
          pthread_mutex_lock(&ptmutex1);
          _path = true;
          cout<<"valor de path"<<_path<<endl;
          pthread_mutex_unlock(&ptmutex1);
      }
  }catch(boost::python::error_already_set const &){}
  cout<<"valor de path"<<_path<<endl;
  return NULL;
}

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

Scene::Scene()
{
  _grid_d=0;
  _center[0]=0.0;
  _center[1]=0.0;
  _fin[0]=0.0;
  _fin[1]=0.0;
  _lock = 0;
  pthread_mutex_init(&ptmutex1,NULL);
  _path = false;
  _objetos.clear();
  _objs.clear();
  _marcas.clear();
  _arobots.clear();
  double p_center[2] = {0.0, 0.0};
  char p_patt[] = "data/4x4_26.patt";
  _marcas.push_back(Marca(30.0,p_center,p_patt));
  _marcas[0].setVisible(false);
  //if((_marcas[0].getId()) < 0) return -1;

  strcpy(p_patt,"data/4x4_78.patt");
  _marcas.push_back(Marca(30.0,p_center,p_patt));
  _marcas[1].setVisible(false);
  //if((_marcas[1].getId()) < 0) return -1;

  strcpy(p_patt,"data/simple.patt");
  _marcas.push_back(Marca(50.0,p_center,p_patt));
  _marcas[2].setVisible(false);
  //if((_marcas[2].getId()) < 0) return -1;

  strcpy(p_patt,"data/4x4_95.patt");
  _marcas.push_back(Marca(50.0,p_center,p_patt));
  _marcas[3].setVisible(false);
  //if((_marcas[3].getId()) < 0) return -1;

  strcpy(p_patt,"data/4x4_85.patt");
  _marcas.push_back(Marca(50.0,p_center,p_patt));
  _marcas[4].setVisible(false);
  //if((_marcas[4].getId()) < 0) return -1;

  _arobots.push_back(ARobot(0));
  _arobots.push_back(ARobot(1));
  //_robots[2] = Robot();

}

int Scene::getLock(){
    return _lock;
}
void Scene::setLock(int id){
    _lock = id;
}

int Scene::getObjeto(int id){
    int obj = -1;
    for (uint i=0; i < _objetos.size(); i++){
        if (_objetos[i].getRobot() == id){
            obj = i;
            break;
        }
    }
    return obj;
}

int Scene::getPrioritario(){
    int obj = 0;
    int prioridad = -1;
    for (uint i=0; i < _objetos.size(); i++){
        if (_objetos[i].getPrioridad() > prioridad){
            obj = i;
            prioridad = _objetos[obj].getPrioridad();
        }
    }
    return obj;
}

void Scene::addObject(Objeto ob){
    _objs.push_back(ob);
}

void Scene::actualizaColores(){
    for (uint i=0; i < _objs.size(); i++){
        int p = pertenece(_objs[i]);
        if (p >= 0){
            _objs[i] = _objetos[p];
        }
    }
    _objetos.clear();
    for (uint i=0; i < _objs.size(); i++){
        _objetos.push_back(_objs[i]);
    }
    _objs.clear();
}

void Scene::clearColors(){
    _objetos.clear();
}

void Scene::clearObjs(){
    _objs.clear();
}

int Scene::pertenece(Objeto ob){
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
  for (int j=0;j<3;j++)
    this->_arobots[j] = s._arobots[j];
  return *this;
}
