
#include "Objeto.h"

Objeto::Objeto(){
}

Objeto::Objeto(int id, int prioridad, double c[2]){

  _id = id;
  _prioridad = prioridad;
  _robot = 0;
  _pos[0] = c[0];
  _pos[1] = c[1];
}
int Objeto::getId(){
    return _id;
}
int Objeto::getPrioridad(){
    return _prioridad;
}
int Objeto::getRobot(){
    return _robot;
}
void Objeto::getPos(double p[2]){
    p[0] = _pos[0];
    p[1] = _pos[1];
}

void Objeto::setPos(double p[2]){
    _pos[0] = p[0];
    _pos[1] = p[1];
}
void Objeto::setRobot(int robot){
    _robot = robot;
}
void Objeto::setPrioridad(int prioridad){
    _prioridad = prioridad;
}
void Objeto::setId(int id){
    _id = id;
}


Objeto& Objeto::operator = (const Objeto &o)
{
    if(this!=&o){ //Comprueba que no se esté intentando igualar un objeto a sí mismo
        this->_id = o._id;
        this->_prioridad = o._prioridad;
        this->_robot = o._robot;
        this->_pos[0] = o._pos[0];
        this->_pos[1] = o._pos[1];
    }
    return *this;
}

bool Objeto::operator == (const Objeto &o)
{
    return this->_id == o._id;
}
