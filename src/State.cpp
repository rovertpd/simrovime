#include "State.h"

State::State(unsigned int x, unsigned int y, movimientos mov){
	_x = x;
	_y = y;
	_mov = mov;
}

State::~State(){}

unsigned int State::getX() const{
	return _x;
}

unsigned int State::getY() const{
	return _y;
}

movimientos State::getMov() const{
	return _mov;
}

State * State::operator =(const State * e){
	return new State(e->getX(),e->getY(),e->getMov());
}
