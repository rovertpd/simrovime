#include "IA.h"

IA::IA(Estado *e,ARobot *agente){
    _state=e;
    _agent=agente;
}

void IA::run(){
    cout<<"IA:: "<<endl;
          _agent->setPath(false);
//          cout<<"posx: "<<pos[0]<<endl;
//          cout<<"ratio: "<<ratio<<endl;
//          cout<<"posx de estado: "<<e->get_posx()<<endl;
          try{
              object mainobj = import("__main__");
              object dictionary(mainobj.attr("__dict__"));
              object result;
              cout<<"IA:: Antes del Result"<<endl;
              result = exec_file("IA.py",dictionary, dictionary);
              cout<<"IA:: Despues del Result"<<endl;
              object busqueda = dictionary["Ejecutar"];
              cout<<"IA:: Busqueda"<<endl;
              if(!busqueda.is_none()){
                  cout<<"IA:: Entra en if"<<endl;
                boost::shared_ptr<Estado> estado(_state);
                cout<<"IA:: Despues de Estado"<<endl;
                  busqueda(ptr(estado.get()));
                  cout<<"IA:: Despues de get estado busqueda"<<endl;
                  Estado *obj = ptr(estado.get());
                  cout<<"IA:: Despues de get estado"<<endl;
                  std::vector<char> aux = obj->get_mov();
                cout<<"IA:: Antes del for"<<endl;
                  for(std::vector<char>::iterator it = aux.begin(); it != aux.end(); it++)
                      _agent->addMov(*it);
                cout<<"IA:: Despues del for"<<endl;
              }
              _agent->setPath(true);
          }catch(boost::python::error_already_set const &){}
//          cout<<"valor de path"<<agente->getPath()<<endl;
}
