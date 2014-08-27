#include "AI.h"

AI::AI(Search *b,ARobot *agente){
    _Search=b;
    _agent=agente;
}

void AI::run(){
    cout<<"AI:: "<<_agent->getId()<<endl;
    _Search->init();
      _agent->setPath(false);
      cout<<"Antes de ejecutar"<<endl;
      _Search->ejecutar();
      cout<<"despues de ejecutar"<<endl;
      cout<<"Has Solution"<<_Search->haySolucion()<<endl;
      vector<State*> aux = _Search->getSolucion();
      for(std::vector<State*>::iterator it = aux.begin(); it != aux.end(); it++){
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
