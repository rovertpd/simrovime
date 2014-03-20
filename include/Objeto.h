#ifndef OBJETO_H
    #define OBJETO_H


class Objeto {
 private:
  int _id;                      // Identificador del patron
  int _prioridad;               // Prioridad del objeto (color)
  int _robot;                   // Id del robot asociado;
  double _pos[2];

 public:
  Objeto();
  Objeto(int id, int prioridad, double c[2]);
  void setId(int id);
  void setPrioridad(int prioridad);
  void setRobot(int robot);
  void setPos(double p[2]);
  int getId();
  int getPrioridad();
  int getRobot();
  void getPos(double p[2]);
  Objeto& operator = (const Objeto &o);
  bool operator == (const Objeto &o);
};
#endif
