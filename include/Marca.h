#ifndef MARCA_H
    #define MARCA_H

#include <AR/ar.h>
#include <AR/gsub.h>
#include <AR/param.h>

class Marca {
 private:
  int _id;                      // Identificador del patron
  int _visible;                 // Es visible el objeto?
  double _width;                // Ancho del patron
  double _center[2];            // Centro del patron
  double _pattTrans[3][4];      // Matriz asociada al patron
  double _pos[2];               // Posicion de la marca
  double _pmax[2];              // Punto maximo
  double _pmin[2];              // Punto minimo
  float _rotation;              // Rotacion de la marca
  ARMarkerInfo _markerInfo;

 public:
  Marca();
  Marca(double w, double c[2], char *p);
  int getId();
  int getVisible();
  double getWidth();
  double* getCenter();
  void getPattTans(double m[3][4]);
  double* getPos();
  ARMarkerInfo* getMarkerInfo();
  void setRot(float r);
  float getRot();
  double* getMax();
  double* getMin();
  void setMax(double* max);
  void setMin(double* min);
  void setMarkerInfo(ARMarkerInfo markerInfo);
  void setPos(double p[2]);
  void setId(int id);
  void setVisible(bool visible);
  void setPattTans(double m[3][4]);
  Marca& operator = (const Marca &m);
};
#endif
