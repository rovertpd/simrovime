#include "Coordinador.h"
#define NR 1 // Nuebo robot
#define NO 4 // Nuevo objeto
#define MR 2 // Movimiento de robot
#define MO 5 // Movimiento de objeto
#define BR 3 // Un robot menos
#define BO 6 // Un objeto menos


Coordinador::Coordinador(){
}

Coordinador::Coordinador(Scene *scn){
  _scene = scn;
  _lock = -1;
  _arobots.clear();
  _scene->attach(this);
  _aRo[0] = false;
  _aRo[1] = false;
  _aRo[2] = false;
  _arobots.push_back(new ARobot(0));
  _arobots.push_back(new ARobot(1));
  _arobots.push_back(new ARobot(2));
}

int Coordinador::anyIdle(){
    int idle = -1;
    for (int i=0; i<3; i++){
        if (!getARobot(i)->hasObj()){
            idle = i;
            break;
        }
    }
    return idle;
}

int Coordinador::havePriority(Objeto* ob){
    int priority = ob->getPrioridad();
    int rob = -1;
    for (vector<ARobot*>::iterator it = _arobots.begin(); it != _arobots.end(); ++it) {
        if ((*it)->getObj()->getPrioridad() < priority){
            priority = (*it)->getObj()->getPrioridad();
            rob = (*it)->getId();
        }
    }
    return rob;
}

void Coordinador::Actualizar(int event, int id){
    if (event == NR){
        cout<<"Coordinador:: NR"<<endl;
        id = id - 2;
        int ob = -1;
        _aRo[id] = true;
        if ((ob = getPrioritario()) != -1){
            cout<<"Coordinador:: "<<ob<<endl;
            getARobot(id)->setObj(_scene->getObjetos()[ob]);
                    getARobot(id)->planifica(_scene,1);
        }else{
                    getARobot(id)->planifica(_scene,3);
        }
    }else if(event == NO){
        cout<<"Coordinador:: NO"<<endl;
        int rob = -1;
        if ((rob = anyIdle()) != -1){
            getARobot(rob)->setObj(_scene->getObjetos().back());
            getARobot(rob)->planifica(_scene,1);
        }else{
            if ((rob = havePriority(_scene->getObjetos().back())) != -1){
                getARobot(rob)->setObj(_scene->getObjetos().back());
                getARobot(rob)->planifica(_scene,1);
            }
        }
    }else if (event == MR){
        cout<<"Coordinador:: MR"<<endl;
        id = id - 2;
        getARobot(id)->planifica(_scene,2);
    }else if (event == MO){
        cout<<"Coordinador:: MO"<<endl;
        for (vector<ARobot*>::iterator it = _arobots.begin(); it != _arobots.end(); ++it) {
            if ((*it)->getObj()->getId() < id){
                            (*it)->planifica(_scene,1);
            }
        }
    }else if (event == BR){
        cout<<"Coordinador:: BR"<<endl;
        id = id - 2;
        getARobot(id)->reset();
    }else if (event == BO){
        cout<<"Coordinador:: BO"<<endl;
        for (vector<ARobot*>::iterator it = _arobots.begin(); it != _arobots.end(); ++it) {
            if ((*it)->getObj()->getId() < id){
                (*it)->deleteObj();
                int ob = -1;
                if ((ob = getPrioritario()) != -1){
                    (*it)->setObj(_scene->getObjetos()[ob]);
                            (*it)->planifica(_scene,1);
                }else{
                            (*it)->planifica(_scene,3);
                }
            }
        }
    }
//    for (int i=2; i<5; i++){
//        if ((_scene->getMarca(i))->getVisible()&&(getLock()==0 || getLock()==i)){
//            ARobot* arobot = getARobot(i-2);
//
////                cout<<"Aqui 66"<<_path<<endl;
////                if (_path){
////                    cout<<"Aqui 11"<<endl;
////                    _path1 =false;
////                    _path = false;
////                    for(std::vector<char>::iterator it = _lMov.begin(); it != _lMov.end(); it++)
////                        cout<<"Accion: "<<*it<<endl;
////                        cout<<"Aqui 22"<<endl;
////                }else if (!_path1){
////                    cout<<"Aqui 33"<<endl;
////                    int arg[] = {i-2, getMarca(i)->getMarkerInfo()->pos[0], getMarca(i)->getMarkerInfo()->pos[1], _fin[0], _fin[1]};
////                    cout<<"XXXXXXX: "<<arg[1]<<endl;
////                    cout<<"Aqui 44"<<endl;
////                    if (pthread_create( &mithread001, NULL, &Scene::Busca,(void *)arg ) ) {
////                        printf("Error creando el hilo.");
////                        abort();
////                    }
////                    _path1 = true;
////                }
//            double fin[2];
//            arobot->getFin(fin);
//            if (fin[0] == -1.0){ //Inicializamos las posiciones de la trayectoria (los puntos finales)
//                arobot->planifica((_scene->getMarca(i))->getMarkerInfo(),_scene->getCenter(),_scene->getFin(),0,_scene->getRotacion(i));
//            }
//            std::cout<<"Robot: "<<i<<" "<< "getLock: "<<getLock()<<std::endl;
//            setLock(i);
//            if ((_scene->getObjetos()).size() > 0){
//                std::cout<<"Hay objetos de color: "<<(_scene->getObjetos()).size()<<std::endl;
//                int idObjeto = getObjeto(i);  //Compruebo si el robot i tiene algún objeto asignado y devuelvo su posición
//                std::cout<<"Hay objetos asignado: "<<getObjeto(i)<<std::endl;
//                if (idObjeto >= 0){
//                    std::cout<<"Tengo un objeto asignado: "<<idObjeto<<std::endl;
//                    double* posicionO = (arobot->getObj())->getPos();
//
//                    arobot->setFin(posicionO);
//                    //cout<<"Objeto x: "<<posicionO[0]<<" Objeto y: "<<posicionO[1]<<endl;
//                    double m[2];
//                    float v[2];
//                    m[0] = (_scene->getMarca(i))->getMarkerInfo()->pos[0];
//                    m[1] = (_scene->getMarca(i))->getMarkerInfo()->pos[1];
//                    v[0] = posicionO[0] - ((_scene->getMarca(i))->getMarkerInfo()->pos[0]);
//                    v[1] = posicionO[1] - ((_scene->getMarca(i))->getMarkerInfo()->pos[1]);
//                    float angulo = _scene->getAngulo(v);
//                    arobot->setAng(angulo);
//                    double dist01 = sqrt(pow((posicionO[0]-m[0]),2)+pow((posicionO[1]-m[1]),2));
//                    if (dist01 > 75){
//                        arobot->planifica((_scene->getMarca(i))->getMarkerInfo(),_scene->getCenter(),_scene->getFin(),3,_scene->getRotacion(i));
//                    }else{  //Cerca del objeto (Distancia < limite)
//                        setLock(0);
//                        arobot->detener();
//                    }
//                }else{  //No hay color asignado
//                    std::cout<<"No tengo objeto asignado"<<std::endl;
//                    if((_scene->getObjetos()).size() >= (i-2)){
//                        std::cout<<"Hay suficientes objetos de color "<<std::endl;
//                        int prioritario = getPrioritario();
//                        arobot->setObj(&((_scene->getObjetos())[prioritario]));
//                        double* pO = (_scene->getObjetos())[prioritario].getPos();
//                        arobot->setFin(pO);
//                        double m[2];
//                        float v[2];
//                        m[0] = (_scene->getMarca(i))->getMarkerInfo()->pos[0];
//                        m[1] = (_scene->getMarca(i))->getMarkerInfo()->pos[1];
//                        v[0] = pO[0] - ((_scene->getMarca(i))->getMarkerInfo()->pos[0]);
//                        v[1] = pO[1] - ((_scene->getMarca(i))->getMarkerInfo()->pos[1]);
//                        float angulo = _scene->getAngulo(v);
//                        arobot->setAng(angulo);
//                        double dist01 = sqrt(pow((pO[0]-m[0]),2)+pow((pO[1]-m[1]),2));
//                        if (dist01 > 75){
//                            arobot->planifica((_scene->getMarca(i))->getMarkerInfo(),_scene->getCenter(),_scene->getFin(),2,_scene->getRotacion(i));
//                        }else{  //Cerca del objeto (Distancia < limite)
//                            setLock(0);
//                            arobot->detener();
//                        }
//                    }
//                }
//            }else{
//                std::cout<<"Robot: "<<i<<" "<< " no tiene objeto a vigilar"<<std::endl;
//                double fin[2];
//                setLock(0);
//                if (arobot->getEst()>4){
//                    arobot->planifica((_scene->getMarca(i))->getMarkerInfo(),_scene->getCenter(),_scene->getFin(),1,_scene->getRotacion(i));
//                    arobot->setEst(0);
//                }else{
//                    double posicionO[2];
//                    arobot->getFin(posicionO);
//                    double m[2];
//                    float v[2];
//                    m[0] = (_scene->getMarca(i))->getMarkerInfo()->pos[0];
//                    m[1] = (_scene->getMarca(i))->getMarkerInfo()->pos[1];
//                    v[0] = posicionO[0] - ((_scene->getMarca(i))->getMarkerInfo()->pos[0]);
//                    v[1] = posicionO[1] - ((_scene->getMarca(i))->getMarkerInfo()->pos[1]);
//                    float angulo = _scene->getAngulo(v);
//                    arobot->setAng(angulo);
//                    double dist01 = sqrt(pow((posicionO[0]-m[0]),2)+pow((posicionO[1]-m[1]),2));
//                    if (dist01 > 75){
//                        arobot->planifica((_scene->getMarca(i))->getMarkerInfo(),_scene->getCenter(),_scene->getFin(),4,_scene->getRotacion(i));
//                    }else{
//                        switch(arobot->getDir()){
//                            case 4:
//                                arobot->setDir(1);
//                                fin[0] = _scene->getFin()[0];
//                                fin[1] = _scene->getFin()[1];
//                                arobot->setFin(fin);
//                            break;
//                            case 3:
//                                arobot->setDir(4);
//                                fin[0] = _scene->getCenter()[0];
//                                fin[1] = _scene->getFin()[1];
//                                arobot->setFin(fin);
//                            break;
//                            case 2:
//                                arobot->setDir(3);
//                                fin[0] = _scene->getCenter()[0];
//                                fin[1] = _scene->getCenter()[1];
//                                arobot->setFin(fin);
//                            break;
//                            case 1:
//                                arobot->setDir(2);
//                                fin[0] = _scene->getFin()[0];
//                                fin[1] = _scene->getCenter()[1];
//                                arobot->setFin(fin);
//                            break;
//                        }
//                    }
//                }
//            }
//        }
//    }
}

int Coordinador::getLock(){
    return _lock;
}

void Coordinador::setLock(int id){
    _lock = id;
}

int Coordinador::getObjeto(int id){
    int obj = -1;
    if (_arobots[id]->hasObj()){
      obj = _arobots[id]->getObj()->getId();
    }
    return obj;
}

int Coordinador::getPrioritario(){
    int idA1, idA2, idA3;
    if (_arobots[0]->hasObj())
      idA1 = _arobots[0]->getObj()->getId();
    if (_arobots[1]->hasObj())
      idA2 = _arobots[1]->getObj()->getId();
    if (_arobots[2]->hasObj())
      idA3 = _arobots[2]->getObj()->getId();
    vector<Objeto*> objs = _scene->getObjetos();
    int obj = -1;
    int prioridad = -1;
    for (uint i=0; i < objs.size(); i++){
        if ((objs[i]->getPrioridad() > prioridad) && (objs[i]->getId() != idA1) && (objs[i]->getId() != idA2) && (objs[i]->getId() != idA3)){
            obj = i;
            prioridad = objs[obj]->getPrioridad();
        }
    }
    return obj;
}

ARobot* Coordinador::getARobot(int id){
    return _arobots[id];
}
