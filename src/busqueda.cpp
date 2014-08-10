#include "busqueda.h"

Busqueda::Busqueda(int id, int ** m, int posRX, int posRY, int posIX, int posIY, int tamX, int tamY, int ratioX, int ratioY, tiposEstrategias es, int col, int posFX, int posFY){
	_mapa = m;
	_id = id;
	_posActX = posIX;
	_posActY = posIY;
	_posRX = posRX;
	_posRY = posRY;
	_tamX = tamX;
	_tamY = tamY;
	_color = col;
	_ratioX = ratioX;
	_ratioY = ratioY;
	_nElem = 0;
	_posFinX = posFX;
	_posFinY = posFY;
	_estrategia = es;
}

void Busqueda::init(){
    _sol = false;
	_visitados = new int*[_tamX];
	for(int i = 0; i < _tamX; i++){
        _visitados[i] = new int[_tamY];
		for(int j = 0; j < _tamY; j++)
			_visitados[i][j] = -1;
	}
}

vector<Estado*> Busqueda::getSolucion() const{
	return _solucion;
}

int Busqueda::costePosicion(const int posX, const int posY) {
	return 3;
}

bool Busqueda::fin(){
    for (int x=_posActX-_ratioX;x<_posActX+_ratioX+1;x++){
        for (int y=_posActY-_ratioY;y<_posActY+_ratioY+1;y++){
            if (x>=0 && x<=_tamX && y>=0 && y<=_tamY){
                if (_color == _id){
                    if (x == _posFinX && y == _posFinY){
                        return true;
                    }
                }else{
                    if (_mapa[x][y]==_color){
                        return true;
                    }
                }
            }
        }
    }
	return false;
}

double Busqueda::estrategia(const int coste, const int posX, const int posY) const{
	return -(coste + heuristica(posX,posY));
}

double Busqueda::heuristica(const int posX, const int posY) const{
	switch(_estrategia){
		//coste = distancia entre el punto X y el punto FIN * coste para llegar
		case AEUCLIDEA:
			return sqrt(pow(posX - _posFinX,2) + pow(posY - _posFinY,2));// * costeMinimo();
		case AMANHATTAN:
			return (abs(posX - _posFinX) + abs(posY - _posFinY));// * costeMinimo();
		default:
			return 0;
	}
	return -1;
}

int Busqueda::costeMinimo() const{
	return 1;
}

bool Busqueda::puedoGenerar(movimientos mov, int * c, int * f, int * coste){
    int inicio [6];
    int fin [6];
    if (_posRX<0.5){
        if (_posRY<0.5){
            if (_ratioX%2==0){
                if (_ratioY%2==0){
                    inicio [0] = *c-(_ratioX/2);
                    inicio [1] = *c-(_ratioX/2)-1;
                    inicio [2] = *c-(_ratioX/2)+1;
                    inicio [3] = *f-(_ratioY/2);
                    inicio [4] = *f-(_ratioY/2)-1;
                    inicio [5] = *f-(_ratioY/2)+1;
                    fin [0] = *c+(_ratioX/2)-1;
                    fin [1] = *c+(_ratioX/2)-2;
                    fin [2] = *c+(_ratioX/2);
                    fin [3] = *f+(_ratioY/2)-1;
                    fin [4] = *f+(_ratioY/2)-2;
                    fin [5] = *f+(_ratioY/2);
                }else{
                    inicio [0] = *c-(_ratioX/2);
                    inicio [1] = *c-(_ratioX/2)-1;
                    inicio [2] = *c-(_ratioX/2)+1;
                    inicio [3] = *f-(_ratioY/2);
                    inicio [4] = *f-(_ratioY/2)-1;
                    inicio [5] = *f-(_ratioY/2)+1;
                    fin [0] = *c+(_ratioX/2)-1;
                    fin [1] = *c+(_ratioX/2)-2;
                    fin [2] = *c+(_ratioX/2);
                    fin [3] = *f+(_ratioY/2);
                    fin [4] = *f+(_ratioY/2)-1;
                    fin [5] = *f+(_ratioY/2)+1;
                }
            }else{
                if (_ratioY%2==0){
                    inicio [0] = *c-(_ratioX/2);
                    inicio [1] = *c-(_ratioX/2)-1;
                    inicio [2] = *c-(_ratioX/2)+1;
                    inicio [3] = *f-(_ratioY/2);
                    inicio [4] = *f-(_ratioY/2)-1;
                    inicio [5] = *f-(_ratioY/2)+1;
                    fin [0] = *c+(_ratioX/2);
                    fin [1] = *c+(_ratioX/2)-1;
                    fin [2] = *c+(_ratioX/2)+1;
                    fin [3] = *f+(_ratioY/2)-1;
                    fin [4] = *f+(_ratioY/2)-2;
                    fin [5] = *f+(_ratioY/2);
                }else{
                    inicio [0] = *c-(_ratioX/2);
                    inicio [1] = *c-(_ratioX/2)-1;
                    inicio [2] = *c-(_ratioX/2)+1;
                    inicio [3] = *f-(_ratioY/2);
                    inicio [4] = *f-(_ratioY/2)-1;
                    inicio [5] = *f-(_ratioY/2)+1;
                    fin [0] = *c+(_ratioX/2);
                    fin [1] = *c+(_ratioX/2)-1;
                    fin [2] = *c+(_ratioX/2)+1;
                    fin [3] = *f+(_ratioY/2);
                    fin [4] = *f+(_ratioY/2)-1;
                    fin [5] = *f+(_ratioY/2)+1;
                }
            }
        }else{
            if (_ratioX%2==0){
                if (_ratioY%2==0){
                    inicio [0] = *c-(_ratioX/2);
                    inicio [1] = *c-(_ratioX/2)-1;
                    inicio [2] = *c-(_ratioX/2)+1;
                    inicio [3] = *f-(_ratioY/2)+1;
                    inicio [4] = *f-(_ratioY/2);
                    inicio [5] = *f-(_ratioY/2)+2;
                    fin [0] = *c+(_ratioX/2)-1;
                    fin [1] = *c+(_ratioX/2)-2;
                    fin [2] = *c+(_ratioX/2);
                    fin [3] = *f+(_ratioY/2);
                    fin [4] = *f+(_ratioY/2)-1;
                    fin [5] = *f+(_ratioY/2)+1;
                }else{
                    inicio [0] = *c-(_ratioX/2);
                    inicio [1] = *c-(_ratioX/2)-1;
                    inicio [2] = *c-(_ratioX/2)+1;
                    inicio [3] = *f-(_ratioY/2);
                    inicio [4] = *f-(_ratioY/2)-1;
                    inicio [5] = *f-(_ratioY/2)+1;
                    fin [0] = *c+(_ratioX/2)-1;
                    fin [1] = *c+(_ratioX/2)-2;
                    fin [2] = *c+(_ratioX/2);
                    fin [3] = *f+(_ratioY/2);
                    fin [4] = *f+(_ratioY/2)-1;
                    fin [5] = *f+(_ratioY/2)+1;
                }
            }else{
                if (_ratioY%2==0){
                    inicio [0] = *c-(_ratioX/2);
                    inicio [1] = *c-(_ratioX/2)-1;
                    inicio [2] = *c-(_ratioX/2)+1;
                    inicio [3] = *f-(_ratioY/2)+1;
                    inicio [4] = *f-(_ratioY/2);
                    inicio [5] = *f-(_ratioY/2)+2;
                    fin [0] = *c+(_ratioX/2);
                    fin [1] = *c+(_ratioX/2)-1;
                    fin [2] = *c+(_ratioX/2)+1;
                    fin [3] = *f+(_ratioY/2);
                    fin [4] = *f+(_ratioY/2)-1;
                    fin [5] = *f+(_ratioY/2)+1;
                }else{
                    inicio [0] = *c-(_ratioX/2);
                    inicio [1] = *c-(_ratioX/2)-1;
                    inicio [2] = *c-(_ratioX/2)+1;
                    inicio [3] = *f-(_ratioY/2);
                    inicio [4] = *f-(_ratioY/2)-1;
                    inicio [5] = *f-(_ratioY/2)+1;
                    fin [0] = *c+(_ratioX/2);
                    fin [1] = *c+(_ratioX/2)-1;
                    fin [2] = *c+(_ratioX/2)+1;
                    fin [3] = *f+(_ratioY/2);
                    fin [4] = *f+(_ratioY/2)-1;
                    fin [5] = *f+(_ratioY/2)+1;
                }
            }
        }
    }else{
        if (_posRY<0.5){
            if (_ratioX%2==0){
                if (_ratioY%2==0){
                    inicio [0] = *c-(_ratioX/2)+1;
                    inicio [1] = *c-(_ratioX/2);
                    inicio [2] = *c-(_ratioX/2)+2;
                    inicio [3] = *f-(_ratioY/2);
                    inicio [4] = *f-(_ratioY/2)-1;
                    inicio [5] = *f-(_ratioY/2)+1;
                    fin [0] = *c+(_ratioX/2);
                    fin [1] = *c+(_ratioX/2)-1;
                    fin [2] = *c+(_ratioX/2)+1;
                    fin [3] = *f+(_ratioY/2)-1;
                    fin [4] = *f+(_ratioY/2)-2;
                    fin [5] = *f+(_ratioY/2);
                }else{
                    inicio [0] = *c-(_ratioX/2)+1;
                    inicio [1] = *c-(_ratioX/2);
                    inicio [2] = *c-(_ratioX/2)+2;
                    inicio [3] = *f-(_ratioY/2);
                    inicio [4] = *f-(_ratioY/2)-1;
                    inicio [5] = *f-(_ratioY/2)+1;
                    fin [0] = *c+(_ratioX/2);
                    fin [1] = *c+(_ratioX/2)-1;
                    fin [2] = *c+(_ratioX/2)+1;
                    fin [3] = *f+(_ratioY/2);
                    fin [4] = *f+(_ratioY/2)-1;
                    fin [5] = *f+(_ratioY/2)+1;
                }
            }else{
                if (_ratioY%2==0){
                    inicio [0] = *c-(_ratioX/2);
                    inicio [1] = *c-(_ratioX/2)-1;
                    inicio [2] = *c-(_ratioX/2)+1;
                    inicio [3] = *f-(_ratioY/2);
                    inicio [4] = *f-(_ratioY/2)-1;
                    inicio [5] = *f-(_ratioY/2)+1;
                    fin [0] = *c+(_ratioX/2);
                    fin [1] = *c+(_ratioX/2)-1;
                    fin [2] = *c+(_ratioX/2)+1;
                    fin [3] = *f+(_ratioY/2)-1;
                    fin [4] = *f+(_ratioY/2)-2;
                    fin [5] = *f+(_ratioY/2);
                }else{
                    inicio [0] = *c-(_ratioX/2);
                    inicio [1] = *c-(_ratioX/2)-1;
                    inicio [2] = *c-(_ratioX/2)+1;
                    inicio [3] = *f-(_ratioY/2);
                    inicio [4] = *f-(_ratioY/2)-1;
                    inicio [5] = *f-(_ratioY/2)+1;
                    fin [0] = *c+(_ratioX/2);
                    fin [1] = *c+(_ratioX/2)-1;
                    fin [2] = *c+(_ratioX/2)+1;
                    fin [3] = *f+(_ratioY/2);
                    fin [4] = *f+(_ratioY/2)-1;
                    fin [5] = *f+(_ratioY/2)+1;
                }
            }
        }else{
            if (_ratioX%2==0){
                if (_ratioY%2==0){
                    inicio [0] = *c-(_ratioX/2)+1;
                    inicio [1] = *c-(_ratioX/2);
                    inicio [2] = *c-(_ratioX/2)+2;
                    inicio [3] = *f-(_ratioY/2)+1;
                    inicio [4] = *f-(_ratioY/2);
                    inicio [5] = *f-(_ratioY/2)+2;
                    fin [0] = *c+(_ratioX/2);
                    fin [1] = *c+(_ratioX/2)-1;
                    fin [2] = *c+(_ratioX/2)+1;
                    fin [3] = *f+(_ratioY/2);
                    fin [4] = *f+(_ratioY/2)-1;
                    fin [5] = *f+(_ratioY/2)+1;
                }else{
                    inicio [0] = *c-(_ratioX/2)+1;
                    inicio [1] = *c-(_ratioX/2);
                    inicio [2] = *c-(_ratioX/2)+2;
                    inicio [3] = *f-(_ratioY/2);
                    inicio [4] = *f-(_ratioY/2)-1;
                    inicio [5] = *f-(_ratioY/2)+1;
                    fin [0] = *c+(_ratioX/2);
                    fin [1] = *c+(_ratioX/2)-1;
                    fin [2] = *c+(_ratioX/2)+1;
                    fin [3] = *f+(_ratioY/2);
                    fin [4] = *f+(_ratioY/2)-1;
                    fin [5] = *f+(_ratioY/2)+1;
                }
            }else{
                if (_ratioY%2==0){
                    inicio [0] = *c-(_ratioX/2);
                    inicio [1] = *c-(_ratioX/2)-1;
                    inicio [2] = *c-(_ratioX/2)+1;
                    inicio [3] = *f-(_ratioY/2)+1;
                    inicio [4] = *f-(_ratioY/2);
                    inicio [5] = *f-(_ratioY/2)+2;
                    fin [0] = *c+(_ratioX/2);
                    fin [1] = *c+(_ratioX/2)-1;
                    fin [2] = *c+(_ratioX/2)+1;
                    fin [3] = *f+(_ratioY/2);
                    fin [4] = *f+(_ratioY/2)-1;
                    fin [5] = *f+(_ratioY/2)+1;
                }else{
                    inicio [0] = *c-(_ratioX/2);
                    inicio [1] = *c-(_ratioX/2)-1;
                    inicio [2] = *c-(_ratioX/2)+1;
                    inicio [3] = *f-(_ratioY/2);
                    inicio [4] = *f-(_ratioY/2)-1;
                    inicio [5] = *f-(_ratioY/2)+1;
                    fin [0] = *c+(_ratioX/2);
                    fin [1] = *c+(_ratioX/2)-1;
                    fin [2] = *c+(_ratioX/2)+1;
                    fin [3] = *f+(_ratioY/2);
                    fin [4] = *f+(_ratioY/2)-1;
                    fin [5] = *f+(_ratioY/2)+1;
                }
            }
        }
    }
	switch(mov){
		case NORTE:
			for (int x =inicio[3];x<fin[3];x++){
                if (x<0 || x>=_tamX || inicio[1]<0 || inicio[1]>=_tamY)
                    return false;
                else
                    if (_mapa[x][inicio[1]]!=-1)
                        return false;
			}
			*c = _posActX;
			*f = _posActY - 1;
			break;

		case SUR:
			for (int x =inicio[3];x<fin[3];x++){
                if (x<0 || x>=_tamX || fin[2]<0 || fin[2]>=_tamY)
                    return false;
                else
                    if (_mapa[x][fin[2]]!=-1)
                        return false;
			}
			*c = _posActX;
			*f = _posActY + 1;
			break;

		case ESTE:
			for (int y =inicio[0];y<fin[0];y++){
                if (y<0 || y>=_tamY || fin[5]<0 || fin[5]>=_tamX)
                    return false;
                else
                    if (_mapa[fin[5]][y]!=-1)
                        return false;
			}
			*c = _posActX + 1;
			*f = _posActY;
			break;

		case OESTE:
			for (int y =inicio[0];y<fin[0];y++){
                if (y<0 || y>=_tamY || inicio[4]<0 || inicio[4]>=_tamX)
                    return false;
                else
                    if (_mapa[inicio[4]][y]!=-1)
                        return false;
			}
			*c = _posActX - 1;
			*f = _posActY;
			break;

        case NESTE:
			for (int x =inicio[5];x<fin[5];x++){
                if (x<0 || x>=_tamX || inicio[1]<0 || inicio[1]>=_tamY)
                    return false;
                else
                    if (_mapa[x][inicio[1]]!=-1)
                        return false;
			}
			for (int y =inicio[1];y<fin[1];y++){
                if (y<0 || y>=_tamY || fin[5]<0 || fin[5]>=_tamX)
                    return false;
                else
                    if (_mapa[fin[5]][y]!=-1)
                        return false;
			}
			*c = _posActX + 1;
			*f = _posActY - 1;
			break;

        case NOESTE:
			for (int x =inicio[4];x<fin[4];x++){
                if (x<0 || x>=_tamX || inicio[1]<0 || inicio[1]>=_tamY)
                    return false;
                else
                    if (_mapa[x][inicio[1]]!=-1)
                        return false;
			}
			for (int y =inicio[1];y<fin[1];y++){
                if (y<0 || y>=_tamY || inicio[4]<0 || inicio[4]>=_tamX)
                    return false;
                else
                    if (_mapa[inicio[4]][y]!=-1)
                        return false;
			}
			*c = _posActX - 1;
			*f = _posActY - 1;
			break;

        case SESTE:
			for (int x =inicio[5];x<fin[5];x++){
                if (x<0 || x>=_tamX || fin[2]<0 || fin[2]>=_tamY)
                    return false;
                else
                    if (_mapa[x][fin[2]]!=-1)
                        return false;
			}
			for (int y =inicio[2];y<fin[2];y++){
                if (y<0 || y>=_tamY || fin[5]<0 || fin[5]>=_tamX)
                    return false;
                else
                    if (_mapa[fin[5]][y]!=-1)
                        return false;
			}
			*c = _posActX + 1;
			*f = _posActY + 1;
			break;

        case SOESTE:
			for (int x =inicio[4];x<fin[4];x++){
                if (x<0 || x>=_tamX || fin[2]<0 || fin[2]>=_tamY)
                    return false;
                else
                    if (_mapa[x][fin[2]]!=-1)
                        return false;
			}
			for (int y =inicio[2];y<fin[2];y++){
                if (y<0 || y>=_tamY || inicio[4]<0 || inicio[4]>=_tamX)
                    return false;
                else
                    if (_mapa[inicio[4]][y]!=-1)
                        return false;
			}
			*c = _posActX - 1;
			*f = _posActY + 1;
			break;
	}
    *coste += costePosicion(*c, *f); //Actualizo el coste
    if(*c>=0 && *c<_tamX && *f>=0 && *f<_tamY)
        if(!visitado(*c, *f, *coste)) //Comprobar visitado
            return true;
	return false;
}

void Busqueda::visitar(int posX, int posY, int coste){
	_visitados[posX][posY] = coste + heuristica(posX, posY);
}

bool Busqueda::visitado(int posX, int posY, int coste) const{
	if(_visitados[posX][posY] != -1){
		//Si esta visitado, compruebo si he llegado a la misma posicion con un coste menor
		//Si el coste es mayor, se considera que esta visitado
		//Si el coste es menor, la posicion no esta visitada
		if(_visitados[posX][posY] <= coste + heuristica(posX, posY)) return true;
	}
	return false;
}

void Busqueda::ejecutar(){
	int coste, posAuxX, posAuxY;
	bool expandir = true;
	int mov;
	Nodo candidato(new Estado(_posActX, _posActY, NORTE), 0, 0, 0);
	visitar(_posActX, _posActY, candidato.getCoste());
	_frontera.push(candidato);
	while(expandir){
		candidato = _frontera.top();
		_frontera.pop();
		_posActX = candidato.getEstado()->getX();
		_posActY = candidato.getEstado()->getY();
		expandir = !fin();
		if(expandir){
			for(int i = 0; i < 8; i++){ //8 tipos de movimientos
				mov = i;
				coste = candidato.getCoste();
				if(puedoGenerar(static_cast<movimientos>(mov), &posAuxX, &posAuxY, &coste)){ //Tambien se comprueba si esta visitada
					//Genero
					//Al generar un hijo, el valor f quedara ordenado de mayor a menor en la cola
					//por lo que en funcion de la estrategia, se establece el valor
					Nodo hijo(new Estado(posAuxX, posAuxY, static_cast<movimientos>(mov)), coste,
											&candidato, estrategia(coste, posAuxX, posAuxY));
					_frontera.push(hijo);
					visitar(posAuxX, posAuxY, coste);
					_nElem++;
				}
			}
		}
	}
	if(fin()){
		_sol = true;
		generarSolucion(&candidato);
	}
}

bool Busqueda::haySolucion() const{
	return _sol;
}

void Busqueda::generarSolucion(Nodo * sol){
	_costeSol = sol->getCoste();
	while(sol->getPadre()){
        _solucion.push_back(sol->getEstado());
        sol = sol->getPadre();
	}
	//_solucion.push_back(sol->getEstado());
}

int Busqueda::getNelem() const{
	return _nElem;
}

int Busqueda::getCosteSol() const{
	return _costeSol;
}

