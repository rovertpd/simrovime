#ifndef NODO_H
#define NODO_H

#include "State.h"

class Nodo{
	private:
		State * _State;
		int _coste;
		Nodo * _padre;
		double _f;
	public:
		Nodo(State * s, const int c, Nodo * p, const double f);
		Nodo();
		~Nodo();
		State * getState() const;
		int getCoste() const;
		Nodo * getPadre() const;
		double getF() const;
		bool tienePadre() const;

		bool operator <(const Nodo & n)const;
		Nodo & operator =(const Nodo & n);
};
#endif
