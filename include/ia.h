#include <IceUtil/Thread.h>
#include "estado.h"
#include "ARobot.h"
#include <boost/lexical_cast.hpp>
#include <boost/python/str.hpp>
#include <vector>


class ia:public IceUtil::Thread{
    virtual void run(int id, int pos[2],int fin[2],int grid[2],vector<int> map,ARobot *agente){
          Estado * e = new Estado(id,pos[0],pos[1],grid[0],grid[1],fin[0],fin[1],map);
          agente->setPath(false);
          cout<<"posx: "<<pos[0]<<endl;
          cout<<"posx de estado: "<<e->get_posx()<<endl;
          try{
              object mainobj = import("__main__");
              object dictionary(mainobj.attr("__dict__"));
              object result;
              result = exec_file("IA.py",dictionary, dictionary);
              object busqueda = dictionary["Ejecutar"];
              if(!busqueda.is_none()){
                boost::shared_ptr<Estado> estado(e);
                  busqueda(ptr(estado.get()));
                  Estado *obj = ptr(estado.get());
                  std::vector<char> aux = obj->get_mov();
                  for(std::vector<char>::iterator it = aux.begin(); it != aux.end(); it++)
                      agente->addMov(*it);
                  agente->setPath(true);
              }
          }catch(boost::python::error_already_set const &){}
          cout<<"valor de path"<<agente->getPath()<<endl;
    }
};
