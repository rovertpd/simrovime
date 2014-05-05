#ifndef Filter_H
 #define Filter_H

using namespace std;


class Filter {
 public:
  static Filter* Instance();
  bool rect_sup(double x, double y);
  bool rect_inf(double x, double y);
  bool rect_der(double x, double y);
  bool rect_izq(double x, double y);
  int getGrid();
  void setGrid(int g);
  void setPsup(double x, double y);
  void setPinf(double x, double y);
 protected:
  Filter();
  Filter &operator= (const Filter &f) ;
 private:
  double _p_sup[2];
  double _p_inf[2];
  int _grid;
  static Filter* pinstance;
};
#endif
