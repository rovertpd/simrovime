#ifndef BUSQUEDA_H
#define BUSQUEDA_H

#include <vector>
#include "nodo.h"
#include <queue>
#include <cstdlib>
#include <cmath>
#include <iostream>

using namespace std;

enum tiposEstrategias{
	AEUCLIDEA,
	AMANHATTAN,
};

class Busqueda{
	private:
		enum tiposEstrategias _estrategia;
		int ** _mapa;
		int ** _visitados;
		vector<Estado*> _solucion;
		int _posRX, _posRY, _posFinX, _posFinY, _tamX, _tamY, _posActX, _posActY, _nElem, _color, _ratioX, _ratioY, _id;
		priority_queue<Nodo> _frontera;
		bool _sol;
		int _costeSol;
	public:
		Busqueda(int id, int ** m, int posRX, int posRY, int posIX, int posIY, int tamX, int tamY, int ratioX, int ratioY, tiposEstrategias es, int col, int posFX, int posFY);
		void init();
		int costePosicion(const int posX, const int posY);
		bool fin();
		double estrategia(const int coste, const int posX, const int posY) const;
		bool puedoGenerar(movimientos mov, int * posX, int * posY, int * coste);
		void visitar(int posX, int posY, int coste);
		bool visitado(int posX, int posY, int coste) const;
		double heuristica(const int posX, const int posY) const;
		int costeMinimo() const;

		vector<Estado*> getSolucion() const;
		int getNelem() const;
		int getCosteSol() const;
		bool haySolucion() const;
		void generarSolucion(Nodo * sol);
		void ejecutar();
};
#endif

