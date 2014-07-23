#ifndef NODO_H
#define NODO_H

#include "estado.h"

class Nodo{
	private:
		Estado * _estado;
		int _coste;
		Nodo * _padre;
		double _f;
	public:
		Nodo(Estado * s, const int c, Nodo * p, const double f);
		Nodo();
		~Nodo();
		Estado * getEstado() const;
		int getCoste() const;
		Nodo * getPadre() const;
		double getF() const;
		bool tienePadre() const;

		bool operator <(const Nodo & n)const;
		Nodo & operator =(const Nodo & n);
};
#endif
