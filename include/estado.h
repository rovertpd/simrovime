#ifndef ESTADO_H
#define ESTADO_H

enum movimientos{
	NORTE,
	SUR,
	ESTE,
	OESTE,
	NESTE,
	NOESTE,
	SESTE,
	SOESTE,
};

class Estado{
	private:
		unsigned int _x; //j
		unsigned int _y; //i
		movimientos _mov;
	public:
		Estado(const unsigned int x, const unsigned int y, movimientos mov);
		~Estado();
		unsigned int getX() const;
		unsigned int getY() const;
		movimientos getMov() const;
		Estado * operator =(const Estado * e);
};
#endif

