#include "Marca.h"

Marca::Marca(){
}

Marca::Marca(double w, double c[2], char *p){

  int pattid;

  if((pattid=arLoadPatt(p)) < 0){
        printf("Error en carga de patron\n");
        _id=-1;
  }else
        _id = pattid;                      // Identificador del patron
  _visible = false;                 // Es visible el objeto?
  _width = w;                // Ancho del patron
  _center[0] = c[0];
  _center[1] = c[1];;            // Centro del patron
}
int Marca::getId(){
    return _id;
}
int Marca::getVisible(){
    return _visible;
}
double Marca::getWidth(){
    return _width;
}
double* Marca::getCenter(){
    return _center;
}
void Marca::getPattTans(double m[3][4]){
    for (int i=0; i<3; ++i)
        for (int j=0; j<4; ++j)
            m[i][j] = _pattTrans[i][j];
}
int* Marca::getPos(){
    return _pos;
}
void Marca::setPos(double p[2]){
    _pos[0]=static_cast<int>(p[0]);
    _pos[1]=static_cast<int>(p[1]);
}
void Marca::setId(int id){
    _id=id;
}
void Marca::setVisible(bool visible){
    _visible=visible;
}
void Marca::setPattTans(double m[3][4]){
    for (int i=0; i<3; ++i)
        for (int j=0; j<4; ++j)
            _pattTrans[i][j] = m[i][j];
    _pattTrans[1][3] = _pattTrans[1][3] * (-1);
}
void Marca::setMarkerInfo(ARMarkerInfo markerInfo){
    _markerInfo=markerInfo;
}
void Marca::setRot(float r){
    _rotation = r;
}

int* Marca::getMax(){
    return _pmax;
}

int* Marca::getMin(){
    return _pmin;
}

int Marca::getRatio(){
    return _ratio;
}

void Marca::setRatio(int ratio){
    _ratio = ratio;
}

void Marca::setMax(int* max){
    _pmax[0]=max[0];
    _pmax[1]=max[1];
}

void Marca::setMin(int* min){
    _pmin[0]=min[0];
    _pmin[1]=min[1];
}

float Marca::getRot(){
    return _rotation;
}
ARMarkerInfo* Marca::getMarkerInfo(){
    return &_markerInfo;
}
Marca& Marca::operator = (const Marca &m)
{
    if(this!=&m){ //Comprueba que no se esté intentanod igualar un objeto a sí mismo
        this->_id = m._id;
        this->_visible = m._visible;
        this->_width = m._width;
        this->_center[0] = m._center[0];
        this->_center[1] = m._center[1];
        for (int i=0; i<3; ++i)
            for (int j=0; j<4; ++j)
                this->_pattTrans[i][j] = m._pattTrans[i][j];
        this->_pos[0] = m._pos[0];
        this->_pos[1] = m._pos[1];
        this->_markerInfo = m._markerInfo;
    }
    return *this;
}
