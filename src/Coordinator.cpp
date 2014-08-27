#include "Coordinator.h"
#define NR 1 // Nuebo robot
#define NO 4 // Nuevo objeto
#define MR 2 // Movimiento de robot
#define MO 5 // Movimiento de objeto
#define BR 3 // Un robot menos
#define BO 6 // Un objeto menos


Coordinator::Coordinator(){
}

Coordinator::Coordinator(Scene *scn){
  _scene = scn;
  _lock = -1;
  _arobots.clear();
}

void Coordinator::init(){
  _scene->attach(this);
  _aRo[0] = false;
  _aRo[1] = false;
  _aRo[2] = false;
  _arobots.push_back(new ARobot(0));
  _arobots.push_back(new ARobot(1));
  _arobots.push_back(new ARobot(2));
  _arobots[0]->init();
  _arobots[1]->init();
  _arobots[2]->init();
}

int Coordinator::anyIdle(){
    int idle = -1;
    for (int i=0; i<3; i++){
        if (!getARobot(i)->hasObj() && _scene->getMarca(i+2)->getVisible()){
            idle = i;
            break;
        }
    }
    return idle;
}

int Coordinator::havePriority(Objeto* ob){
    int priority = ob->getPrioridad();
    int rob = -1;
    for (vector<ARobot*>::iterator it = _arobots.begin(); it != _arobots.end(); ++it) {
        if (_scene->getMarca((*it)->getId()+2)->getVisible()){
            if ((*it)->getObj()->getPrioridad() < priority){
                priority = (*it)->getObj()->getPrioridad();
                rob = (*it)->getId();
            }
        }
    }
    return rob;
}

void Coordinator::Actualizar(int event, int id){
    if (event == NR){
        cout<<"Coordinator:: NR"<<endl;
        id = id - 2;
        int ob = -1;
        _aRo[id] = true;
        if ((ob = getPrioritario()) != -1){
            cout<<"Coordinator:: "<<ob<<endl;
            getARobot(id)->setObj(_scene->getObjetos()[ob]);
            getARobot(id)->planifica(_scene,1);
        }else{
            getARobot(id)->planifica(_scene,3);
        }
    }else if(event == NO){
        cout<<"Coordinator:: NO"<<endl;
        int rob = -1;
        if ((rob = anyIdle()) != -1){
            cout<< "Idle"<<endl;
            getARobot(rob)->setObj(_scene->getObjetos().back());
            getARobot(rob)->planifica(_scene,1);
        }else{
            if ((rob = havePriority(_scene->getObjetos().back())) != -1){
                cout<< "Priority"<<endl;
                getARobot(rob)->setObj(_scene->getObjetos().back());
                getARobot(rob)->planifica(_scene,1);
            }
        }
    }else if (event == MR){
        cout<<"Coordinator:: MR"<<endl;
        id = id - 2;
        int gc = getColision( id ) ;
        if ( gc == 0) { // No existe riesgo colisión
            getARobot( id )->planifica( _scene ,2) ;
            // Movimiento normal
        }
        else if ( gc == -1) {
        // Hay riesgo de colisión pero tengo la prioridad
            if ( getARobot( id )->getGuardia() == -1) {
                // Recalcula la ruta según estado
                getARobot( id )->planifica( _scene ,1) ;
                // Vigila objeto
            } else {
                getARobot( id )->planifica( _scene ,3) ;
                //Guardia
            }
        } else {
        // Hay riesgo y además no tengo la prioridad
            getARobot( id )->detener() ;
        // Me detengo hasta nueva orden
        }

    }else if (event == MO){
        cout<<"Coordinator:: MO"<<endl;
        for (vector<ARobot*>::iterator it = _arobots.begin(); it != _arobots.end(); ++it) {
            if (_scene->getMarca((*it)->getId()+2)->getVisible()){
                if ((*it)->hasObj()){
                    if ((*it)->getObj()->getId() == id){
                        (*it)->planifica(_scene,1);
                    }
                }
            }
        }
    }else if (event == BR){
        cout<<"Coordinator:: BR"<<endl;
        id = id - 2;
        if ( getARobot( id )->hasObj() ) { // Si vigilaba un objeto, se busca nuevo vigilante
            int rob = -1;
            rob = anyIdle();
            if ( rob != -1) {
                // Si hay algún robot ocioso
                getARobot( rob )->setObj(_scene->getObjetos().back()) ;
                getARobot( rob )->planifica(_scene ,1) ;
                // Vigilar objeto
            } else {
                rob = havePriority(_scene->getObjetos().back());
                if ( rob != -1) {
                // Si hay un robot vigilando un objeto menos prioritario
                getARobot( rob )->setObj( _scene->getObjetos().back () ) ;
                getARobot( rob )->planifica( _scene ,1) ; // Vigilar objeto
                }
            }
        }
        getARobot(id)->reset();
    }else if (event == BO){
        cout<<"Coordinator:: BO"<<endl;
        for (vector<ARobot*>::iterator it = _arobots.begin(); it != _arobots.end(); ++it) {
            if (_scene->getMarca((*it)->getId()+2)->getVisible()){
                if ((*it)->hasObj()){
                    if ((*it)->getObj()->getId() == id){
                        (*it)->deleteObj();
                        int ob = getPrioritario();
                        if (ob != -1){
                            (*it)->setObj(_scene->getObjetos()[ob]);
                            (*it)->planifica(_scene,1);
                        }else{
                            (*it)->planifica(_scene,3);
                        }
                    }
                }
            }
        }
    }
}

int Coordinator::getObjeto(int id){
    int obj = -1;
    if (_arobots[id]->hasObj()){
      obj = _arobots[id]->getObj()->getId();
    }
    return obj;
}

int Coordinator::getPrioritario(){
    int idA1=-1, idA2=-1, idA3=-1;
    if (_arobots[0]->hasObj())
      idA1 = _arobots[0]->getObj()->getId();
    if (_arobots[1]->hasObj())
      idA2 = _arobots[1]->getObj()->getId();
    if (_arobots[2]->hasObj())
      idA3 = _arobots[2]->getObj()->getId();
    vector<Objeto*> objs = _scene->getObjetos();
    int obj = -1;
    int prioridad = -1;
    for (uint i=0; i < objs.size(); i++){
        if ((objs[i]->getPrioridad() > prioridad) && (objs[i]->getId() != idA1) && (objs[i]->getId() != idA2) && (objs[i]->getId() != idA3)){
            obj = i;
            prioridad = objs[obj]->getPrioridad();
        }
    }
    return obj;
}

ARobot* Coordinator::getARobot(int id){
    return _arobots[id];
}

int Coordinator::getColision(int id){
    for (int x=0;x<3;x++){
        if (_arobots[x]->getId()!=id){
            if (_scene->getMarca(x+2)->getVisible()){
                if (sqrt(pow(_scene->getMarca(x+2)->getPos()[0]-_scene->getMarca(id+2)->getPos()[0],2)+pow(_scene->getMarca(x+2)->getPos()[1]-_scene->getMarca(id+2)->getPos()[1],2))<(_scene->getGrid()*4)){
                    if (_arobots[id]->hasObj()){
                        if (_arobots[x]->hasObj()){
                            if (_arobots[id]->getObj()->getId()<_arobots[x]->getObj()->getId()){
                                return 1;
                            }
                        }
                    }else{
                        if (_arobots[x]->hasObj()){
                            return 1;
                        }
                    }
                    return -1;
                }
            }
        }
    }
    return 0;
}
