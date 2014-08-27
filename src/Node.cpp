#include "Node.h"

Nodo::Nodo(State * e, int c, Nodo * p, double f){
	_State = e;
	_coste = c;
	_padre = p;
	_f = f;
}

Nodo::Nodo(){
	_State = 0;
	_coste = 999999999;
	_padre = 0;
	_f = 0;
}

Nodo::~Nodo(){
}

State * Nodo::getState() const { return _State; }
int Nodo::getCoste() const { return _coste; }
Nodo * Nodo::getPadre() const { return _padre; }
double Nodo::getF() const { return _f; }
bool Nodo::tienePadre() const { return _padre ? true : false; }

bool Nodo::operator <(const Nodo & n)const{ return _f < n.getF(); }

Nodo & Nodo::operator =(const Nodo & n){
	_State = new State(n.getState()->getX(), n.getState()->getY(), n.getState()->getMov());
	_coste = n.getCoste();
	if(n.getPadre())
		_padre = new Nodo(n.getPadre()->getState(),n.getPadre()->getCoste(),n.getPadre()->getPadre(),n.getPadre()->getF());
	else
		_padre = 0;
	_f = n.getF();
	return *this;
}
