#include "IA.h"

IA::IA(Busqueda *b,ARobot *agente){
    _busqueda=b;
    _agent=agente;
}

void IA::run(){
    cout<<"IA:: "<<_agent->getId()<<endl;
          _agent->setPath(false);
          cout<<"Antes de ejecutar"<<endl;
          _busqueda->ejecutar();
          cout<<"despues de ejecutar"<<endl;
          cout<<"Has Solution"<<_busqueda->haySolucion()<<endl;
          vector<Estado*> aux = _busqueda->getSolucion();
          for(std::vector<Estado*>::iterator it = aux.begin(); it != aux.end(); it++){
              switch((*it)->getMov()){
                  case 0:
                    _agent->addMov('U');
                  break;
                  case 1:
                    _agent->addMov('D');
                  break;
                  case 2:
                    _agent->addMov('R');
                  break;
                  case 3:
                    _agent->addMov('L');
                  break;
                  case 4:
                    _agent->addMov('E');
                  break;
                  case 5:
                    _agent->addMov('O');
                  break;
                  case 6:
                    _agent->addMov('I');
                  break;
                  case 7:
                    _agent->addMov('W');
                  break;
              }
              cout<<"Mov"<<(*it)->getMov()<<endl;
          }
          _agent->setPath(true);
}
