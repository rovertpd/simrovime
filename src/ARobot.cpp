#include "ARobot.h"

ARobot::ARobot(int id){
    _pos[0] = 0.0;
    _pos[1] = 0.0;
    _ang = 0.0;
    _fin[0] = -1.0;
    _fin[1] = -1.0;
    _id = id;
    _direccion = 0;
    _robot = new Robot(_id);
}

int ARobot::getId(){
    return _id;
}

int ARobot::getDir(){
    return _direccion;
}

void ARobot::setDir(int dir){
    _direccion = dir;
}

void ARobot::getFin(double f[2]){
    f[0] = _fin[0];
    f[1] = _fin[1];
}

double ARobot::getAng(){
    return _ang;
}

int ARobot::getEst(){
    return _estado;
}

void ARobot::setEst(int est){
    _estado = est;
}

void ARobot::setAng(double ang){
    _ang = ang;
}

void ARobot::setFin(double f[2]){
    _fin[0] = f[0];
    _fin[1] = f[1];
}

void ARobot::detener(){
    _robot->parar();
    setEst(7);
}

void ARobot::planifica(ARMarkerInfo *markerInfo,double inicio[2],double fin[2],int accion,double rotacion){
    _markerInfo = markerInfo;
    _pos[0] = _markerInfo->pos[0];
    _pos[1] = _markerInfo->pos[1];
    std::cout<<"Rotacion: "<<rotacion<<std::endl;
    std::cout<<"Angulo final: "<<_ang<<std::endl;
    std::cout<<"Posicion: ["<<_pos[0]<<","<<_pos[1]<<"]"<<std::endl;
    std::cout<<"Posicion final: ["<<_fin[0]<<","<<_fin[1]<<"]"<<std::endl;
    std::cout<<"Accion: "<<accion<<std::endl;
    if ((accion == 0) || (accion == 1)){
        double final[2];
        if (_pos[0] > ((fin[0] - inicio[0]) / 2 + inicio[0])){
            if (_pos[1] > ((fin[1] - inicio[1]) / 2 + inicio[1])){
                setDir(1);
                final[0] = fin[0];
                final[1] = fin[1];
                setFin(final);
            }else{
                setDir(2);
                final[0] = fin[0];
                final[1] = inicio[1];
                setFin(final);
            }
        }else{
            if (_pos[1] > ((fin[1] - inicio[1]) / 2 + inicio[1])){
                setDir(4);
                final[0] = inicio[0];
                final[1] = fin[1];
                setFin(final);
            }else{
                setDir(3);
                final[0] = inicio[0];
                final[1] = inicio[1];
                setFin(final);
            }
        }
    }else{
        if (rotacion>(_ang+10) || rotacion<(_ang-10)){
            if (_ang>rotacion){
                if (_ang-rotacion<180){
                    _robot->izquierda();
                }
                else{
                    _robot->derecha();
                }
            }else{
                if (_ang-rotacion<180){
                    _robot->derecha();
                }
                else{
                    _robot->izquierda();
                }
            }
            if (accion == 4)
                setEst(2);
            else
                setEst(6);
        }else{  //Ya está orientado (Dentro del umbral)
            _robot->avanzar();
            if (accion == 4)
                setEst(1);
            else
                setEst(5);
        }
    }
}

ARobot& ARobot::operator= (const ARobot &r){
    this->_fin[0] = r._fin[0];
    this->_fin[1] = r._fin[1];
    this->_pos[0] = r._pos[0];
    this->_pos[1] = r._pos[1];
    this->_ang = r._ang;
    this->_id = r._id;
    this->_direccion = r._direccion;
    this->_robot = r._robot;
    this->_markerInfo = r._markerInfo;
    return *this;
}
