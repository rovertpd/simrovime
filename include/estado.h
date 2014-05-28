#ifndef STATE_H
 #define STATE_H

#include <boost/python/detail/wrap_python.hpp>
#include <boost/python/object.hpp>
#include <boost/python/dict.hpp>
#include <boost/python/exec.hpp>
#include <boost/python/module.hpp>
#include <boost/python/class.hpp>
#include <boost/python/import.hpp>
#include <boost/python/list.hpp>
#include <boost/python/iterator.hpp>
#include <boost/python/errors.hpp>
#include <boost/python/def.hpp>
#include <boost/python.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <vector>


using namespace boost::python;
using namespace std;

namespace py = boost::python;

template<class T>
py::list std_vector_to_py_list(const std::vector<T>& v)
{
    py::object get_iter = py::iterator<std::vector<T> >();
    py::object iter = get_iter(v);
    py::list l(iter);
    return l;
}



class Estado{
  private:
    int _id;
    int _posx, _posy;
    int _ratio;
    int _posxE, _posyE;
    int _gridx, _gridy;
    std::vector<char> _m;
    std::vector<int> _map;
  public:
    Estado(int id, int posx, int posy, int ratio, int gridx, int gridy, int posxE, int posyE, std::vector<int> map){
      _id = id;
      _posx = posx;
      _posy = posy;
      _ratio = ratio;
      _gridx = gridx;
      _gridy = gridy;
      _posxE = posxE;
      _posyE = posyE;
      _map = map;
    }
    int get_id() { return _id; }
    int get_posx() { return _posx; }
    int get_posy() { return _posy; }
    int get_ratio() { return _ratio; }
    int get_gridx() { return _gridx; }
    int get_gridy() { return _gridy; }
    int get_posxE() { return _posxE; }
    int get_posyE() { return _posyE; }
    void add_mov(char m){ _m.push_back(m); }
    std::vector<char> get_mov() { return _m; }
    py::list get_mapa() { return vector_int_to_py_list(_map); }
    py::list vector_int_to_py_list(std::vector<int>& v){
      py::list ret;
      std::vector<int>::iterator it;
      for(it = v.begin(); it != v.end(); it++)
        ret.append((*it));
      return ret;
    }
};
#endif
