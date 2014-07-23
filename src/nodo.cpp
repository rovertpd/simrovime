#include "nodo.h"

Nodo::Nodo(Estado * e, int c, Nodo * p, double f){
	_estado = e;
	_coste = c;
	_padre = p;
	_f = f;
}

Nodo::Nodo(){
	_estado = 0;
	_coste = 999999999;
	_padre = 0;
	_f = 0;
}

Nodo::~Nodo(){
}

Estado * Nodo::getEstado() const { return _estado; }
int Nodo::getCoste() const { return _coste; }
Nodo * Nodo::getPadre() const { return _padre; }
double Nodo::getF() const { return _f; }
bool Nodo::tienePadre() const { return _padre ? true : false; }

bool Nodo::operator <(const Nodo & n)const{ return _f < n.getF(); }

Nodo & Nodo::operator =(const Nodo & n){
	_estado = new Estado(n.getEstado()->getX(), n.getEstado()->getY(), n.getEstado()->getMov());
	_coste = n.getCoste();
	if(n.getPadre())
		_padre = new Nodo(n.getPadre()->getEstado(),n.getPadre()->getCoste(),n.getPadre()->getPadre(),n.getPadre()->getF());
	else
		_padre = 0;
	_f = n.getF();
	return *this;
}
