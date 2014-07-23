#include "estado.h"

Estado::Estado(unsigned int x, unsigned int y, movimientos mov){
	_x = x;
	_y = y;
	_mov = mov;
}

Estado::~Estado(){}

unsigned int Estado::getX() const{
	return _x;
}

unsigned int Estado::getY() const{
	return _y;
}

movimientos Estado::getMov() const{
	return _mov;
}

Estado * Estado::operator =(const Estado * e){
	return new Estado(e->getX(),e->getY(),e->getMov());
}
