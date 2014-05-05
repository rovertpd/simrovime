#include "Coordinador.h"

Coordinador::Coordinador(){
}

Coordinador::Coordinador(Scene *scn){
  _scene = scn;
  _lock = 0;
  _arobots.clear();
  _scene->attach(this);

  _arobots.push_back(ARobot(0));
  _arobots.push_back(ARobot(1));
  //_robots[2] = Robot();
}
void Coordinador::Actualizar(){
    for (int i=2; i<5; i++){
        if ((_scene->getMarca(i))->getVisible()&&(getLock()==0 || getLock()==i)){
            ARobot* arobot;
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
            if (fin[0] == -1.0){ //Inicializamos las posiciones de la trayectoria (los puntos finales)
                arobot->planifica((_scene->getMarca(i))->getMarkerInfo(),_scene->getCenter(),_scene->getFin(),0,_scene->getRotacion(i));
            }
            std::cout<<"Robot: "<<i<<" "<< "getLock: "<<getLock()<<std::endl;
            setLock(i);
            if ((_scene->getObjetos()).size() > 0){
                std::cout<<"Hay objetos de color: "<<(_scene->getObjetos()).size()<<std::endl;
                int nObjeto = _scene->getObjeto(i);  //Compruebo si el robot i tiene algún objeto asignado y devuelvo su posición
                std::cout<<"Hay objetos asignado: "<<_scene->getObjeto(i)<<std::endl;
                if (nObjeto >= 0){
                    std::cout<<"Tengo un objeto asignado: "<<nObjeto<<std::endl;
                    double posicionO[2];
                    (_scene->getObjetos())[nObjeto].getPos(posicionO);

                    arobot->setFin(posicionO);
                    //cout<<"Objeto x: "<<posicionO[0]<<" Objeto y: "<<posicionO[1]<<endl;
                    double m[2];
                    float v[2];
                    m[0] = (_scene->getMarca(i))->getMarkerInfo()->pos[0];
                    m[1] = (_scene->getMarca(i))->getMarkerInfo()->pos[1];
                    v[0] = posicionO[0] - ((_scene->getMarca(i))->getMarkerInfo()->pos[0]);
                    v[1] = posicionO[1] - ((_scene->getMarca(i))->getMarkerInfo()->pos[1]);
                    float angulo = _scene->getAngulo(v);
                    arobot->setAng(angulo);
                    double dist01 = sqrt(pow((posicionO[0]-m[0]),2)+pow((posicionO[1]-m[1]),2));
                    if (dist01 > 75){
                        arobot->planifica((_scene->getMarca(i))->getMarkerInfo(),_scene->getCenter(),_scene->getFin(),3,_scene->getRotacion(i));
                    }else{  //Cerca del objeto (Distancia < limite)
                        setLock(0);
                        arobot->detener();
                    }
                }else{  //No hay color asignado
                    std::cout<<"No tengo objeto asignado"<<std::endl;
                    if((_scene->getObjetos()).size() >= (i-2)){
                        std::cout<<"Hay suficientes objetos de color "<<std::endl;
                        int prioritario = _scene->getPrioritario();
                        (_scene->getObjetos())[prioritario].setRobot(i);
                        double pO[2];
                        (_scene->getObjetos())[prioritario].getPos(pO);
                        arobot->setFin(pO);
                        double m[2];
                        float v[2];
                        m[0] = (_scene->getMarca(i))->getMarkerInfo()->pos[0];
                        m[1] = (_scene->getMarca(i))->getMarkerInfo()->pos[1];
                        v[0] = pO[0] - ((_scene->getMarca(i))->getMarkerInfo()->pos[0]);
                        v[1] = pO[1] - ((_scene->getMarca(i))->getMarkerInfo()->pos[1]);
                        float angulo = _scene->getAngulo(v);
                        arobot->setAng(angulo);
                        double dist01 = sqrt(pow((pO[0]-m[0]),2)+pow((pO[1]-m[1]),2));
                        if (dist01 > 75){
                            arobot->planifica((_scene->getMarca(i))->getMarkerInfo(),_scene->getCenter(),_scene->getFin(),2,_scene->getRotacion(i));
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
                    arobot->planifica((_scene->getMarca(i))->getMarkerInfo(),_scene->getCenter(),_scene->getFin(),1,_scene->getRotacion(i));
                    arobot->setEst(0);
                }else{
                    double posicionO[2];
                    arobot->getFin(posicionO);
                    double m[2];
                    float v[2];
                    m[0] = (_scene->getMarca(i))->getMarkerInfo()->pos[0];
                    m[1] = (_scene->getMarca(i))->getMarkerInfo()->pos[1];
                    v[0] = posicionO[0] - ((_scene->getMarca(i))->getMarkerInfo()->pos[0]);
                    v[1] = posicionO[1] - ((_scene->getMarca(i))->getMarkerInfo()->pos[1]);
                    float angulo = _scene->getAngulo(v);
                    arobot->setAng(angulo);
                    double dist01 = sqrt(pow((posicionO[0]-m[0]),2)+pow((posicionO[1]-m[1]),2));
                    if (dist01 > 75){
                        arobot->planifica((_scene->getMarca(i))->getMarkerInfo(),_scene->getCenter(),_scene->getFin(),4,_scene->getRotacion(i));
                    }else{
                        switch(arobot->getDir()){
                            case 4:
                                arobot->setDir(1);
                                fin[0] = _scene->getFin()[0];
                                fin[1] = _scene->getFin()[1];
                                arobot->setFin(fin);
                            break;
                            case 3:
                                arobot->setDir(4);
                                fin[0] = _scene->getCenter()[0];
                                fin[1] = _scene->getFin()[1];
                                arobot->setFin(fin);
                            break;
                            case 2:
                                arobot->setDir(3);
                                fin[0] = _scene->getCenter()[0];
                                fin[1] = _scene->getCenter()[1];
                                arobot->setFin(fin);
                            break;
                            case 1:
                                arobot->setDir(2);
                                fin[0] = _scene->getFin()[0];
                                fin[1] = _scene->getCenter()[1];
                                arobot->setFin(fin);
                            break;
                        }
                    }
                }
            }
        }
    }
}

int Coordinador::getLock(){
    return _lock;
}

void Coordinador::setLock(int id){
    _lock = id;
}

ARobot* Coordinador::getARobot(int id){
    return &(_arobots[id]);
}
