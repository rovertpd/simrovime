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
  int _pos[2];               // Posicion de la marca
  int _pmax[2];              // Punto maximo
  int _pmin[2];              // Punto minimo
  float _rotation;              // Rotacion de la marca
  int _ratio[2];                    // Radio de la marca para cada robot (mitad en cuadros de grid)
  ARMarkerInfo _markerInfo;

 public:
  Marca();
  Marca(double w, double c[2], char *p);
  int getId();
  int getVisible();
  double getWidth();
  double* getCenter();
  void getPattTans(double m[3][4]);
  int* getPos();
  ARMarkerInfo* getMarkerInfo();
  void setRot(float r);
  float getRot();
  int* getRatio();
  void setRatio(int* ratio);
  int* getMax();
  int* getMin();
  void setMax(int* max);
  void setMin(int* min);
  void setMarkerInfo(ARMarkerInfo markerInfo);
  void setPos(double p[2]);
  void setId(int id);
  void setVisible(bool visible);
  void setPattTans(double m[3][4]);
  Marca& operator = (const Marca &m);
};
#endif
