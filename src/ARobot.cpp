#include "ARobot.h"

#define NEWIA 1
#define MOVIMIENTO 2
#define GUARDIA 3


ARobot::ARobot(int id){
    _pos[0] = 0.0;
    _pos[1] = 0.0;
    _ang = 0.0;
    _fin[0] = -1.0;
    _fin[1] = -1.0;
    _path = false;
    _ob = false;
    _object = NULL;
    _id = id;
    _direccion = 0;
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
    return _estado;
}

void ARobot::setEst(int est){
    _estado = est;
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
//    setEst(7);
}
//grid = tablero
//tam = tamaño de cada casilla

bool ARobot::casillaValida(Scene *scn){
    cout<<"ARobot::"<<_id<<":: Casilla Valida"<<endl;
    if ((scn->getMarca(_id+2)->getPos()[0] == _pos[0]) && (scn->getMarca(_id+2)->getPos()[1]) == _pos[1]){
        return true;
    }else if (_lMov[0] == 'R' && ((scn->getMarca(_id+2)->getPos()[0] +1) == _pos[0]) && (scn->getMarca(_id+2)->getPos()[1]) == _pos[1]){
        return true;
    }else if (_lMov[0] == 'I' && ((scn->getMarca(_id+2)->getPos()[0] +1) == _pos[0]) && (scn->getMarca(_id+2)->getPos()[1]) == (_pos[1] +1)){
        return true;
    }else if (_lMov[0] == 'D' && ((scn->getMarca(_id+2)->getPos()[0]) == _pos[0]) && (scn->getMarca(_id+2)->getPos()[1]) == (_pos[1] +1)){
        return true;
    }else if (_lMov[0] == 'W' && ((scn->getMarca(_id+2)->getPos()[0] -1) == _pos[0]) && (scn->getMarca(_id+2)->getPos()[1]) == (_pos[1] +1)){
        return true;
    }else if (_lMov[0] == 'L' && ((scn->getMarca(_id+2)->getPos()[0] -1) == _pos[0]) && (scn->getMarca(_id+2)->getPos()[1]) == (_pos[1])){
        return true;
    }else if (_lMov[0] == 'O' && ((scn->getMarca(_id+2)->getPos()[0] -1) == _pos[0]) && (scn->getMarca(_id+2)->getPos()[1]) == (_pos[1] -1)){
        return true;
    }else if (_lMov[0] == 'U' && ((scn->getMarca(_id+2)->getPos()[0]) == _pos[0]) && (scn->getMarca(_id+2)->getPos()[1]) == (_pos[1] -1)){
        return true;
    }else if (_lMov[0] == 'E' && ((scn->getMarca(_id+2)->getPos()[0] +1) == _pos[0]) && (scn->getMarca(_id+2)->getPos()[1]) == (_pos[1] -1)){
        return true;
    }else {
        return false;
    }
}

void ARobot::orientar(Scene *scn){
    cout<<"ARobot::"<<_id<<":: Orientar"<<endl;
    float v[2];
    if (_path){
        switch(_lMov.front()){
            case 'R':
                _fin[0] = (scn->getMarca(_id))->getMarkerInfo()->pos[0] + scn->getGrid();
                _fin[1] = (scn->getMarca(_id))->getMarkerInfo()->pos[1];
                break;
            case 'I':
                _fin[0] = (scn->getMarca(_id))->getMarkerInfo()->pos[0] + scn->getGrid();
                _fin[1] = (scn->getMarca(_id))->getMarkerInfo()->pos[1] + scn->getGrid();
                break;
            case 'D':
                _fin[0] = (scn->getMarca(_id))->getMarkerInfo()->pos[0] ;
                _fin[1] = (scn->getMarca(_id))->getMarkerInfo()->pos[1] + scn->getGrid();
                break;
            case 'W':
                _fin[0] = (scn->getMarca(_id))->getMarkerInfo()->pos[0] - scn->getGrid();
                _fin[1] = (scn->getMarca(_id))->getMarkerInfo()->pos[1] + scn->getGrid();
                break;
            case 'L':
                _fin[0] = (scn->getMarca(_id))->getMarkerInfo()->pos[0] - scn->getGrid();
                _fin[1] = (scn->getMarca(_id))->getMarkerInfo()->pos[1] ;
                break;
            case 'O':
                _fin[0] = (scn->getMarca(_id))->getMarkerInfo()->pos[0] - scn->getGrid();
                _fin[1] = (scn->getMarca(_id))->getMarkerInfo()->pos[1] - scn->getGrid();
                break;
            case 'U':
                _fin[0] = (scn->getMarca(_id))->getMarkerInfo()->pos[0] ;
                _fin[1] = (scn->getMarca(_id))->getMarkerInfo()->pos[1] - scn->getGrid();
                break;
            case 'E':
                _fin[0] = (scn->getMarca(_id))->getMarkerInfo()->pos[0] + scn->getGrid();
                _fin[1] = (scn->getMarca(_id))->getMarkerInfo()->pos[1] - scn->getGrid();
                break;
        }
        v[0] = _fin[0] - ((scn->getMarca(_id))->getMarkerInfo()->pos[0]);
        v[1] = _fin[1] - ((scn->getMarca(_id))->getMarkerInfo()->pos[1]);
        _ang = scn->getAngulo(v);

        if (scn->getRotacion(_id)>(_ang+10) || scn->getRotacion(_id)<(_ang-10)){
            if (_ang>scn->getRotacion(_id)){
                if (_ang-scn->getRotacion(_id)<180){
                    _robot->izquierda();
                }
                else{
                    _robot->derecha();
                }
            }else{
                if (_ang-scn->getRotacion(_id)<180){
                    _robot->derecha();
                }
                else{
                    _robot->izquierda();
                }
            }
        }else{  //Ya está orientado (Dentro del umbral)
            if (_lMov.size()!=1)
                _robot->avanzar();
        }
    }
}

void ARobot::nextMov(){
    cout<<"ARobot::"<<_id<<":: Next Mov"<<endl;
    _lMov.pop_back();
}

void ARobot::planifica(Scene *scn,int event){
    _scn=scn;
    if (event == NEWIA){
        cout<<"ARobot::"<<_id<<":: NEWIA"<<endl;
        _lMov.clear();
        Estado *e = new Estado(_id,scn->getMarca(_id+2)->getPos()[0],scn->getMarca(_id+2)->getPos()[1],scn->getMarca(_id+2)->getRatio(),scn->getAncho()/scn->getGrid(),scn->getAlto()/scn->getGrid(),_object->getPos()[0],_object->getPos()[1],scn->getGrid(),scn->getGrid(),scn->arrayToVectorMap());
        IceUtil::ThreadPtr t = new IA(e,this);
        t->start();

    }else if (event == MOVIMIENTO){
        cout<<"ARobot::"<<_id<<":: MOVIMIENTO"<<endl;
        if (casillaValida(scn)){
            if (_lMov.size() == 1){
                orientar(scn);
            }else{
                if ((scn->getMarca(_id+2)->getPos()[0] == _pos[0]) && (scn->getMarca(_id+2)->getPos()[1]) == _pos[1]){
                    orientar(scn);
                }else{
                    nextMov();
                    orientar(scn);
                }
            }
        }else{
            _lMov.clear();
            Estado *e = new Estado(_id,scn->getMarca(_id+2)->getPos()[0],scn->getMarca(_id+2)->getPos()[1],scn->getMarca(_id+2)->getRatio(),scn->getAncho()/scn->getGrid(),scn->getAlto()/scn->getGrid(),_object->getPos()[0],_object->getPos()[1],scn->getGrid(),scn->getGrid(),scn->arrayToVectorMap());
            IceUtil::ThreadPtr t = new IA(e,this);
            t->start();
        }
    }else if (event == GUARDIA){
        cout<<"ARobot::"<<_id<<":: GUARDIA"<<endl;
        _lMov.clear();
        for (int j=0;j<=scn->getAncho()/scn->getGrid();j++){
            for (int i=0;i<=scn->getAlto()/scn->getGrid();i++){
              std::cout<<scn->getMap(i,j);
            }
            std::cout<<""<<std::endl;
          }
        cout<<"ARobot:: Fin: "<<scn->getFin()[0]/scn->getGrid()<<" : "<<scn->getFin()[1]/scn->getGrid()<<endl;
        Estado *e = new Estado(_id,scn->getMarca(_id+2)->getPos()[0],scn->getMarca(_id+2)->getPos()[1],scn->getMarca(_id+2)->getRatio(),scn->getAncho()/scn->getGrid(),scn->getAlto()/scn->getGrid(),scn->getFin()[0]/scn->getGrid(),scn->getFin()[1]/scn->getGrid(),scn->getGrid(),scn->getGrid(),scn->arrayToVectorMap());
        IceUtil::ThreadPtr t = new IA(e,this);
        t->start();
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
//    std::cout<<"Estado: "<<_estado<<" Direccion: "<<_direccion<<std::endl;
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
