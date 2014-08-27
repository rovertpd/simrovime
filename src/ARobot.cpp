#include "ARobot.h"

#define NEWAI 1
#define MOVIMIENTO 2
#define GUARDAI 3


ARobot::ARobot(int id){
    _pos[0] = 0.0;
    _pos[1] = 0.0;
    _ang = 0.0;
    _fin[0] = -1.0;
    _fin[1] = -1.0;
    _path = true;
    _ob = false;
    _object = NULL;
    _id = id;
    _direccion = 0;
    _guardia = -1;
}

void ARobot::init(){
    _robot = new Robot(_id);
}

void ARobot::setPos(double p[2]){
    _pos[0] = p[0];
    _pos[1] = p[1];
}

void ARobot::reset(){
    _pos[0] = 0.0;
    _pos[1] = 0.0;
    _ang = 0.0;
    _fin[0] = -1.0;
    _fin[1] = -1.0;
    _path = false;
    _ob = false;
    _object = NULL;
    _direccion = 0;
//    _guardAI = -1;
}

int ARobot::getId(){
    return _id;
}

int ARobot::getDir(){
    return _direccion;
}

bool ARobot::getPath(){
    return _path;
}

void ARobot::addMov(char mov){
    cout<<"ARobot::"<<_id<<":: Add Mov: "<<mov<<endl;
    _lMov.push_back(mov);
}

void ARobot::setPath(bool path){
    cout<<"ARobot::"<<_id<<":: Set path"<<endl;
    _path = path;
    if (path){
        orientar(_scn);
    }
}

void ARobot::setDir(int dir){
    _direccion = dir;
}

void ARobot::getFin(double f[2]){
    f[0] = _fin[0];
    f[1] = _fin[1];
}

double ARobot::getAng(){
    return _ang;
}

int ARobot::getEst(){
    return _State;
}

void ARobot::setEst(int est){
    _State = est;
}

void ARobot::setAng(double ang){
    _ang = ang;
}

void ARobot::setFin(double f[2]){
    _fin[0] = f[0];
    _fin[1] = f[1];
}

void ARobot::deleteObj(){
    _object = NULL;
    _ob = false;
}

void ARobot::setObj(Objeto *obj){
    _object = obj;
    _ob = true;
}

bool ARobot::hasObj(){
    if (_object!=NULL){
      return true;
    }else {
      return false;
    }
}

Objeto* ARobot::getObj(){
    return _object;
}

void ARobot::detener(){
    cout<<"ARobot::"<<_id<<":: Detener"<<endl;
    _robot->parar();
}

bool ARobot::casillaValida(Scene *scn){
//    cout<<"ARobot::"<<_id<<":: Casilla Valida: "<<_lMov[0]<<" pos:"<<_pos[0]/scn->getGrid()<<":"<<_pos[1]/scn->getGrid()<<" Pos actual:"<<scn->getMarca(_id+2)->getPos()[0]/scn->getGrid()<<":"<<scn->getMarca(_id+2)->getPos()[1]/scn->getGrid()<<endl;
    if ((scn->getMarca(_id+2)->getPos()[0]/scn->getGrid() == static_cast<int>(_pos[0]/scn->getGrid())) && (scn->getMarca(_id+2)->getPos()[1]/scn->getGrid()) == static_cast<int>(_pos[1]/scn->getGrid())){
        return true;
    }else if (_lMov[0] == 'R' && ((scn->getMarca(_id+2)->getPos()[0]/scn->getGrid() +1) == static_cast<int>(_pos[0]/scn->getGrid())) && (scn->getMarca(_id+2)->getPos()[1]/scn->getGrid()) == static_cast<int>(_pos[1]/scn->getGrid())){
        return true;
    }else if (_lMov[0] == 'I' && ((scn->getMarca(_id+2)->getPos()[0]/scn->getGrid() +1) == static_cast<int>(_pos[0]/scn->getGrid())) && (scn->getMarca(_id+2)->getPos()[1]/scn->getGrid() +1) == static_cast<int>(_pos[1]/scn->getGrid())){
        return true;
    }else if (_lMov[0] == 'D' && ((scn->getMarca(_id+2)->getPos()[0]/scn->getGrid()) == static_cast<int>(_pos[0]/scn->getGrid())) && (scn->getMarca(_id+2)->getPos()[1]/scn->getGrid() +1) == static_cast<int>(_pos[1]/scn->getGrid())){
        return true;
    }else if (_lMov[0] == 'W' && ((scn->getMarca(_id+2)->getPos()[0]/scn->getGrid() -1) == static_cast<int>(_pos[0]/scn->getGrid())) && (scn->getMarca(_id+2)->getPos()[1]/scn->getGrid() +1) == static_cast<int>(_pos[1]/scn->getGrid())){
        return true;
    }else if (_lMov[0] == 'L' && ((scn->getMarca(_id+2)->getPos()[0]/scn->getGrid() -1) == static_cast<int>(_pos[0]/scn->getGrid())) && (scn->getMarca(_id+2)->getPos()[1]/scn->getGrid()) == static_cast<int>(_pos[1]/scn->getGrid())){
        return true;
    }else if (_lMov[0] == 'O' && ((scn->getMarca(_id+2)->getPos()[0]/scn->getGrid() -1) == static_cast<int>(_pos[0]/scn->getGrid())) && (scn->getMarca(_id+2)->getPos()[1]/scn->getGrid() -1) == static_cast<int>(_pos[1]/scn->getGrid())){
        return true;
    }else if (_lMov[0] == 'U' && ((scn->getMarca(_id+2)->getPos()[0]/scn->getGrid()) == static_cast<int>(_pos[0]/scn->getGrid())) && (scn->getMarca(_id+2)->getPos()[1]/scn->getGrid() -1) == static_cast<int>(_pos[1]/scn->getGrid())){
        return true;
    }else if (_lMov[0] == 'E' && ((scn->getMarca(_id+2)->getPos()[0]/scn->getGrid() +1) == static_cast<int>(_pos[0]/scn->getGrid())) && (scn->getMarca(_id+2)->getPos()[1]/scn->getGrid() -1) == static_cast<int>(_pos[1]/scn->getGrid())){
        return true;
    }else {
        cout<<"No Valida"<<endl;
        return false;
    }
}

void ARobot::orientar(Scene *scn){
//    cout<<"ARobot::"<<_id<<":: Orientar"<<endl;
    float v[2];
    if (_path){
        switch(_lMov.front()){
            case 'R':
                _fin[0] = (scn->getMarca(_id+2))->getMarkerInfo()->pos[0] + scn->getGrid();
                _fin[1] = (scn->getMarca(_id+2))->getMarkerInfo()->pos[1];
                break;
            case 'I':
                _fin[0] = (scn->getMarca(_id+2))->getMarkerInfo()->pos[0] + scn->getGrid();
                _fin[1] = (scn->getMarca(_id+2))->getMarkerInfo()->pos[1] + scn->getGrid();
                break;
            case 'D':
                _fin[0] = (scn->getMarca(_id+2))->getMarkerInfo()->pos[0] ;
                _fin[1] = (scn->getMarca(_id+2))->getMarkerInfo()->pos[1] + scn->getGrid();
                break;
            case 'W':
                _fin[0] = (scn->getMarca(_id+2))->getMarkerInfo()->pos[0] - scn->getGrid();
                _fin[1] = (scn->getMarca(_id+2))->getMarkerInfo()->pos[1] + scn->getGrid();
                break;
            case 'L':
                _fin[0] = (scn->getMarca(_id+2))->getMarkerInfo()->pos[0] - scn->getGrid();
                _fin[1] = (scn->getMarca(_id+2))->getMarkerInfo()->pos[1] ;
                break;
            case 'O':
                _fin[0] = (scn->getMarca(_id+2))->getMarkerInfo()->pos[0] - scn->getGrid();
                _fin[1] = (scn->getMarca(_id+2))->getMarkerInfo()->pos[1] - scn->getGrid();
                break;
            case 'U':
                _fin[0] = (scn->getMarca(_id+2))->getMarkerInfo()->pos[0] ;
                _fin[1] = (scn->getMarca(_id+2))->getMarkerInfo()->pos[1] - scn->getGrid();
                break;
            case 'E':
                _fin[0] = (scn->getMarca(_id+2))->getMarkerInfo()->pos[0] + scn->getGrid();
                _fin[1] = (scn->getMarca(_id+2))->getMarkerInfo()->pos[1] - scn->getGrid();
                break;
        }
        v[0] = _fin[0] - ((scn->getMarca(_id+2))->getMarkerInfo()->pos[0]);
        v[1] = _fin[1] - ((scn->getMarca(_id+2))->getMarkerInfo()->pos[1]);
//        cout<<_lMov.front()<<"      "<<_guardia<<endl;
//        cout<<"Rotacion: "<<scn->getRotacion(_id+2)<<endl;
        _ang = scn->getAngulo(v);
//        cout<<"ARobot:: Angulo:"<<_ang<<" Rotaciopn actual:"<<scn->getRotacion(_id+2)<<endl;
        if(_ang==0 || _ang==-0){
            if (scn->getRotacion(_id+2)>10 && scn->getRotacion(_id+2)<350){
                if (_ang>=180){
                    if (scn->getRotacion(_id+2) < _ang && scn->getRotacion(_id+2) > _ang-180){
                        _robot->izquierda();
                    }else{
                        _robot->derecha();
                    }
                }else{
                    if (scn->getRotacion(_id+2)>180+_ang || scn->getRotacion(_id+2)<_ang){
                        _robot->izquierda();
                    }else{
                        _robot->derecha();
                    }
                }
            }else{  //Ya está orientado (Dentro del umbral)
                    if (_lMov.size()!=1)
                    _robot->avanzar();
            }
        }else{
            if (scn->getRotacion(_id+2)>(_ang+10) || scn->getRotacion(_id+2)<(_ang-10)){
                if (_ang>=180){
                    if (scn->getRotacion(_id+2) < _ang && scn->getRotacion(_id+2) > _ang-180){
                        _robot->izquierda();
                    }else{
                        _robot->derecha();
                    }
                }else{
                    if (scn->getRotacion(_id+2)>180+_ang || scn->getRotacion(_id+2)<_ang){
                        _robot->izquierda();
                    }else{
                        _robot->derecha();
                    }
                }
            }else{  //Ya está orientado (Dentro del umbral)
                    if (_lMov.size()!=1)
                    _robot->avanzar();
            }
        }
    }
}

void ARobot::orientarO(Scene *scn){
    cout<<"ARobot::"<<_id<<":: OrientarO"<<endl;
    float v[2];
    if (_path){
        switch(_lMov.front()){
            case 'R':
                _fin[0] = (scn->getMarca(_id+2))->getMarkerInfo()->pos[0] + scn->getGrid();
                _fin[1] = (scn->getMarca(_id+2))->getMarkerInfo()->pos[1];
                break;
            case 'I':
                _fin[0] = (scn->getMarca(_id+2))->getMarkerInfo()->pos[0] + scn->getGrid();
                _fin[1] = (scn->getMarca(_id+2))->getMarkerInfo()->pos[1] + scn->getGrid();
                break;
            case 'D':
                _fin[0] = (scn->getMarca(_id+2))->getMarkerInfo()->pos[0] ;
                _fin[1] = (scn->getMarca(_id+2))->getMarkerInfo()->pos[1] + scn->getGrid();
                break;
            case 'W':
                _fin[0] = (scn->getMarca(_id+2))->getMarkerInfo()->pos[0] - scn->getGrid();
                _fin[1] = (scn->getMarca(_id+2))->getMarkerInfo()->pos[1] + scn->getGrid();
                break;
            case 'L':
                _fin[0] = (scn->getMarca(_id+2))->getMarkerInfo()->pos[0] - scn->getGrid();
                _fin[1] = (scn->getMarca(_id+2))->getMarkerInfo()->pos[1] ;
                break;
            case 'O':
                _fin[0] = (scn->getMarca(_id+2))->getMarkerInfo()->pos[0] - scn->getGrid();
                _fin[1] = (scn->getMarca(_id+2))->getMarkerInfo()->pos[1] - scn->getGrid();
                break;
            case 'U':
                _fin[0] = (scn->getMarca(_id+2))->getMarkerInfo()->pos[0] ;
                _fin[1] = (scn->getMarca(_id+2))->getMarkerInfo()->pos[1] - scn->getGrid();
                break;
            case 'E':
                _fin[0] = (scn->getMarca(_id+2))->getMarkerInfo()->pos[0] + scn->getGrid();
                _fin[1] = (scn->getMarca(_id+2))->getMarkerInfo()->pos[1] - scn->getGrid();
                break;
        }
        v[0] = _fin[0] - ((scn->getMarca(_id+2))->getMarkerInfo()->pos[0]);
        v[1] = _fin[1] - ((scn->getMarca(_id+2))->getMarkerInfo()->pos[1]);
        cout<<_lMov.front()<<"      "<<_guardia<<endl;
//        cout<<"Fin :"<<_fin[0]<<"-"<<(scn->getMarca(_id+2))->getMarkerInfo()->pos[0]<<endl;
//        cout<<"Fin :"<<_fin[1]<<"-"<<(scn->getMarca(_id+2))->getMarkerInfo()->pos[1]<<endl;
        _ang = scn->getAngulo(v);
        cout<<"ARobot:: Angulo:"<<_ang<<" Rotaciopn actual:"<<scn->getRotacion(_id+2)<<endl;
        if(_ang==0 || _ang==-0){
            if (scn->getRotacion(_id+2)>(_ang+10) && scn->getRotacion(_id+2)<(350)){
                if (_ang>=180){
                    if (scn->getRotacion(_id+2) < _ang && scn->getRotacion(_id+2) > _ang-180){
                        _robot->izquierda();
                    }else{
                        _robot->derecha();
                    }
                }else{
                    if (scn->getRotacion(_id+2)>180+_ang || scn->getRotacion(_id+2)<_ang){
                        _robot->izquierda();
                    }else{
                        _robot->derecha();
                    }
                }
            }else{  //Ya está orientado (Dentro del umbral)
                    _robot->parar();
            }
        }else{
            if (scn->getRotacion(_id+2)>(_ang+10) || scn->getRotacion(_id+2)<(_ang-10)){
                if (_ang>=180){
                    if (scn->getRotacion(_id+2) < _ang && scn->getRotacion(_id+2) > _ang-180){
                        _robot->izquierda();
                    }else{
                        _robot->derecha();
                    }
                }else{
                    if (scn->getRotacion(_id+2)>180+_ang || scn->getRotacion(_id+2)<_ang){
                        _robot->izquierda();
                    }else{
                        _robot->derecha();
                    }
                }
            }else{  //Ya está orientado (Dentro del umbral)
                    _robot->parar();
            }
        }
    }
}

void ARobot::nextMov(){
//    cout<<"ARobot::"<<_id<<":: Next Mov"<<endl;
    _lMov.pop_back();
}

void ARobot::planifica(Scene *scn,int event){
//    _robot->parar();
    _scn=scn;
    tiposEstrategias es=AMANHATTAN;
    if (event == NEWAI){
        cout<<"ARobot::"<<_id<<":: NEWAI:"<<_object->getPos()[0]<<":"<<_object->getPos()[1]<<endl;
        _lMov.clear();
        _guardia = -1;
        Search *b = new Search(_id, scn->getMap(), scn->getMarca(_id+2)->getPos()[0]%scn->getGrid(), scn->getMarca(_id+2)->getPos()[1]%scn->getGrid(), scn->getMarca(_id+2)->getPos()[0]/scn->getGrid(), scn->getMarca(_id+2)->getPos()[1]/scn->getGrid(), scn->getAncho()/scn->getGrid()+1, scn->getAlto()/scn->getGrid()+1, scn->getMarca(_id+2)->getRatio()[0], scn->getMarca(_id+2)->getRatio()[1], es, _object->getId()+3, _object->getPos()[0]/scn->getGrid(), _object->getPos()[1]/scn->getGrid());
        //Search *b = new Search(_id, scn->getMap(), scn->getMarca(_id+2)->getPos()[0]%scn->getGrid(), scn->getMarca(_id+2)->getPos()[1]%scn->getGrid(), scn->getMarca(_id+2)->getPos()[0]/scn->getGrid(), scn->getMarca(_id+2)->getPos()[1]/scn->getGrid(), scn->getAncho()/scn->getGrid()+1, scn->getAlto()/scn->getGrid()+1, scn->getMarca(_id+2)->getRatio()[0], scn->getMarca(_id+2)->getRatio()[1], es, _id, fin0,fin1);
        //State *e = new State(_id,scn->getMarca(_id+2)->getPos()[0]%scn->getGrid(),scn->getMarca(_id+2)->getPos()[1]%scn->getGrid(),scn->getMarca(_id+2)->getPos()[0]/scn->getGrid(),scn->getMarca(_id+2)->getPos()[1]/scn->getGrid(),scn->getMarca(_id+2)->getRatio()[0],scn->getMarca(_id+2)->getRatio()[1],scn->getAncho()/scn->getGrid()+1,scn->getAlto()/scn->getGrid()+1,_object->getPos()[0],_object->getPos()[1],scn->getGrid(),scn->getGrid(),scn->arrayToVectorMap());
        IceUtil::ThreadPtr t = new AI(b,this);
        t->start();

    }else if (event == MOVIMIENTO){
        cout<<"ARobot::"<<_id<<":: MOVIMIENTO"<<endl;
        if (casillaValida(scn)){
            if ((int)_lMov.size() <= scn->getMarca(_id+2)->getRatio()[0]+1){
                if(_guardia>0){
                    fin0 = -1, fin1 = -1;
                    switch(_guardia){
                        case 1:
                            _guardia = 2;
                            fin0 = scn->getFin()[0]/scn->getGrid();
                            fin1 = scn->getCenter()[1]/scn->getGrid();
                        break;
                        case 2:
                            _guardia = 3;
                            fin0 = scn->getCenter()[0]/scn->getGrid();;
                            fin1 = scn->getCenter()[1]/scn->getGrid();;
                        break;
                        case 3:
                            _guardia = 4;
                            fin0 = scn->getCenter()[0]/scn->getGrid();;
                            fin1 = scn->getFin()[1]/scn->getGrid();
                        break;
                        case 4:
                            _guardia = 1;
                            fin0 = scn->getFin()[0]/scn->getGrid();
                            fin1 = scn->getFin()[1]/scn->getGrid();
                        break;
                    }
                    _lMov.clear();
                    Search *b = new Search(_id, scn->getMap(), scn->getMarca(_id+2)->getPos()[0]%scn->getGrid(), scn->getMarca(_id+2)->getPos()[1]%scn->getGrid(), scn->getMarca(_id+2)->getPos()[0]/scn->getGrid(), scn->getMarca(_id+2)->getPos()[1]/scn->getGrid(), scn->getAncho()/scn->getGrid()+1, scn->getAlto()/scn->getGrid()+1, scn->getMarca(_id+2)->getRatio()[0], scn->getMarca(_id+2)->getRatio()[1], es, _id, fin0,fin1);
                    IceUtil::ThreadPtr t = new AI(b,this);
                    t->start();
//                    cout<<"TIDDDDDD:"<<tc.id()<<endl;
                } else {
                    orientarO(scn);
                }
            }else{
                if ((scn->getMarca(_id+2)->getPos()[0]/scn->getGrid() == static_cast<int>(_pos[0]/scn->getGrid())) && (scn->getMarca(_id+2)->getPos()[1]/scn->getGrid()) == static_cast<int>(_pos[1]/scn->getGrid())){
                    orientar(scn);
                }else{
//                    cout<<"nextm ove"<<endl;
                    nextMov();
                    orientar(scn);
                }
            }
        }else{
            _lMov.clear();
            if(_guardia>0){
                Search *b = new Search(_id, scn->getMap(), scn->getMarca(_id+2)->getPos()[0]%scn->getGrid(), scn->getMarca(_id+2)->getPos()[1]%scn->getGrid(), scn->getMarca(_id+2)->getPos()[0]/scn->getGrid(), scn->getMarca(_id+2)->getPos()[1]/scn->getGrid(), scn->getAncho()/scn->getGrid()+1, scn->getAlto()/scn->getGrid()+1, scn->getMarca(_id+2)->getRatio()[0], scn->getMarca(_id+2)->getRatio()[1], es, _id, fin0,fin1);
                IceUtil::ThreadPtr t = new AI(b,this);
                t->start();
//                cout<<"TIDDDDDD:"<<tc.id()<<endl;
            }else{
                Search *b = new Search(_id, scn->getMap(), scn->getMarca(_id+2)->getPos()[0]%scn->getGrid(), scn->getMarca(_id+2)->getPos()[1]%scn->getGrid(), scn->getMarca(_id+2)->getPos()[0]/scn->getGrid(), scn->getMarca(_id+2)->getPos()[1]/scn->getGrid(), scn->getAncho()/scn->getGrid()+1, scn->getAlto()/scn->getGrid()+1, scn->getMarca(_id+2)->getRatio()[0], scn->getMarca(_id+2)->getRatio()[1], es, _object->getId()+3, _object->getPos()[0]/scn->getGrid(), _object->getPos()[1]/scn->getGrid());
                //State *e = new State(_id,scn->getMarca(_id+2)->getPos()[0]%scn->getGrid(),scn->getMarca(_id+2)->getPos()[1]%scn->getGrid(),scn->getMarca(_id+2)->getPos()[0]/scn->getGrid(),scn->getMarca(_id+2)->getPos()[1]/scn->getGrid(),scn->getMarca(_id+2)->getRatio()[0],scn->getMarca(_id+2)->getRatio()[1],scn->getAncho()/scn->getGrid()+1,scn->getAlto()/scn->getGrid()+1,scn->getFin()[0]/scn->getGrid(),scn->getFin()[1]/scn->getGrid(),scn->getGrid(),scn->getGrid(),scn->arrayToVectorMap());
                IceUtil::ThreadPtr t = new AI(b,this);
                t->start();
            }
        }
    }else if (event == GUARDAI){
        cout<<"ARobot::"<<_id<<":: GUARDAI"<<endl;
        _lMov.clear();
        if (_guardia==-1){
            _guardia = 1;
//        for (int j=0;j<=scn->getAlto()/scn->getGrid();j++){
//            for (int i=0;i<=scn->getAncho()/scn->getGrid();i++){
//              std::cout<<scn->getMap(i,j);
//            }
//            std::cout<<""<<std::endl;
//          }
          fin0=scn->getFin()[0]/scn->getGrid();
          fin1=scn->getFin()[1]/scn->getGrid();
        }
//        cout<<"ARobot:: Inicio: "<<scn->getMarca(_id+2)->getPos()[0]/scn->getGrid()<<" : "<<scn->getMarca(_id+2)->getPos()[1]/scn->getGrid()<<endl;
//        cout<<"ARobot:: ratio: "<<scn->getMarca(_id+2)->getRatio()[0]<<" : "<<scn->getMarca(_id+2)->getRatio()[1]<<endl;
        Search *b = new Search(_id, scn->getMap(), scn->getMarca(_id+2)->getPos()[0]%scn->getGrid(), scn->getMarca(_id+2)->getPos()[1]%scn->getGrid(), scn->getMarca(_id+2)->getPos()[0]/scn->getGrid(), scn->getMarca(_id+2)->getPos()[1]/scn->getGrid(), scn->getAncho()/scn->getGrid()+1, scn->getAlto()/scn->getGrid()+1, scn->getMarca(_id+2)->getRatio()[0], scn->getMarca(_id+2)->getRatio()[1], es, _id, fin0,fin1);
        //State *e = new State(_id,scn->getMarca(_id+2)->getPos()[0]%scn->getGrid(),scn->getMarca(_id+2)->getPos()[1]%scn->getGrid(),scn->getMarca(_id+2)->getPos()[0]/scn->getGrid(),scn->getMarca(_id+2)->getPos()[1]/scn->getGrid(),scn->getMarca(_id+2)->getRatio()[0],scn->getMarca(_id+2)->getRatio()[1],scn->getAncho()/scn->getGrid()+1,scn->getAlto()/scn->getGrid()+1,scn->getFin()[0]/scn->getGrid(),scn->getFin()[1]/scn->getGrid(),scn->getGrid(),scn->getGrid(),scn->arrayToVectorMap());
        IceUtil::ThreadPtr t = new AI(b,this);
        t->start();
//        cout<<"TIDDDDDD:"<<tc.id()<<endl;
    }
    double po[2] = {scn->getMarca(_id+2)->getPos()[0],scn->getMarca(_id+2)->getPos()[1]};
    setPos(po);
//    _markerInfo = markerInfo;
//    _pos[0] = _markerInfo->pos[0];
//    _pos[1] = _markerInfo->pos[1];
//    std::cout<<"Rotacion: "<<rotacion<<std::endl;
//    std::cout<<"Angulo final: "<<_ang<<std::endl;
//    std::cout<<"Posicion: ["<<_pos[0]<<","<<_pos[1]<<"]"<<std::endl;
//    std::cout<<"Posicion final: ["<<_fin[0]<<","<<_fin[1]<<"]"<<std::endl;
//    std::cout<<"State: "<<_State<<" Direccion: "<<_direccion<<std::endl;
//    std::cout<<"Accion: "<<accion<<std::endl;
//    if ((accion == 0) || (accion == 1)){
//        double final[2];
//        if (_pos[0] > ((fin[0] - inicio[0]) / 2 + inicio[0])){
//            if (_pos[1] > ((fin[1] - inicio[1]) / 2 + inicio[1])){
//                setDir(1);
//                final[0] = fin[0];
//                final[1] = fin[1];
//                setFin(final);
//            }else{
//                setDir(2);
//                final[0] = fin[0];
//                final[1] = inicio[1];
//                setFin(final);
//            }
//        }else{
//            if (_pos[1] > ((fin[1] - inicio[1]) / 2 + inicio[1])){
//                setDir(4);
//                final[0] = inicio[0];
//                final[1] = fin[1];
//                setFin(final);
//            }else{
//                setDir(3);
//                final[0] = inicio[0];
//                final[1] = inicio[1];
//                setFin(final);
//            }
//        }
//    }else{
//        if (rotacion>(_ang+10) || rotacion<(_ang-10)){
//            if (_ang>rotacion){
//                if (_ang-rotacion<180){
//                    _robot->izquierda();
//                }
//                else{
//                    _robot->derecha();
//                }
//            }else{
//                if (_ang-rotacion<180){
//                    _robot->derecha();
//                }
//                else{
//                    _robot->izquierda();
//                }
//            }
//            if (accion == 4)
//                setEst(2);
//            else
//                setEst(6);
//        }else{  //Ya está orientado (Dentro del umbral)
//            _robot->avanzar();
//            if (accion == 4)
//                setEst(1);
//            else
//                setEst(5);
//        }
//    }
}

int ARobot::getGuardia(){
    return _guardia;
}

ARobot& ARobot::operator= (const ARobot &r){
    this->_fin[0] = r._fin[0];
    this->_fin[1] = r._fin[1];
    this->_pos[0] = r._pos[0];
    this->_pos[1] = r._pos[1];
    this->_ang = r._ang;
    this->_id = r._id;
    this->_direccion = r._direccion;
    this->_robot = r._robot;
    this->_markerInfo = r._markerInfo;
    return *this;
}
