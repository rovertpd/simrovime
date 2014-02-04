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
  double _pattTrans[3][4];     // Matriz asociada al patron
  double _pos[2];
  ARMarkerInfo _markerInfo;

 public:
  Marca();
  Marca(double w, double c[2], char *p);
  int getId();
  int getVisible();
  double getWidth();
  void getCenter(double c[2]);
  void getPattTans(double m[3][4]);
  void getPos(double p[2]);
  ARMarkerInfo* getMarkerInfo();
  void setMarkerInfo(ARMarkerInfo markerInfo);
  void setPos(double p[2]);
  void setId(int id);
  void setVisible(bool visible);
  void setPattTans(double m[3][4]);
  Marca& operator = (const Marca &m);
};
#endif
