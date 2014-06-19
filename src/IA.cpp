#include "IA.h"

IA::IA(Estado *e,ARobot *agente){
    _state=e;
    _agent=agente;
}

void IA::run(){
          _agent->setPath(false);
//          cout<<"posx: "<<pos[0]<<endl;
//          cout<<"ratio: "<<ratio<<endl;
//          cout<<"posx de estado: "<<e->get_posx()<<endl;
          try{
              object mainobj = import("__main__");
              object dictionary(mainobj.attr("__dict__"));
              object result;
              result = exec_file("IA.py",dictionary, dictionary);
              object busqueda = dictionary["Ejecutar"];
              if(!busqueda.is_none()){
                boost::shared_ptr<Estado> estado(_state);
                  busqueda(ptr(estado.get()));
                  Estado *obj = ptr(estado.get());
                  std::vector<char> aux = obj->get_mov();
                  for(std::vector<char>::iterator it = aux.begin(); it != aux.end(); it++)
                      _agent->addMov(*it);
                  _agent->setPath(true);
              }
          }catch(boost::python::error_already_set const &){}
//          cout<<"valor de path"<<agente->getPath()<<endl;
}
