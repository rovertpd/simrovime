#include "Filter.h"

Filter::Filter(){
}

Filter* Filter::pinstance = 0;// Inicializar el puntero
Filter* Filter::Instance ()
{
  if (pinstance == 0)  // ¿Es la primera llamada?
  {
    pinstance = new Filter; // Creamos la instancia
  }
  return pinstance; // Retornamos la dirección de la instancia
}

bool Filter::rect_sup(double x, double y){
    return (y>_p_sup[1]);
}
bool Filter::rect_inf(double x, double y){
    return (y<_p_inf[1]);
}
bool Filter::rect_der(double x, double y){
    return (x<_p_inf[0]);
}
bool Filter::rect_izq(double x, double y){
    return (x>_p_sup[0]);
}
int Filter::getGrid(){
    return _grid;
}
void Filter::setGrid(int g){
    _grid = g;
}
void Filter::setPsup(double x, double y){
    _p_sup[0] = x;
    _p_sup[1] = y;
}
void Filter::setPinf(double x, double y){
    _p_inf[0] = x;
    _p_inf[1] = y;
}
Filter& Filter::operator=(const Filter &f){
  this->_grid = f._grid;
  this->_p_sup[0] = f._p_sup[0];
  this->_p_sup[1] = f._p_sup[1];
  this->_p_inf[0] = f._p_inf[0];
  this->_p_inf[1] = f._p_inf[1];
  return *this;
}
