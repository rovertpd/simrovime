#ifndef State_H
#define State_H

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

class State{
	private:
		unsigned int _x; //j
		unsigned int _y; //i
		movimientos _mov;
	public:
		State(const unsigned int x, const unsigned int y, movimientos mov);
		~State();
		unsigned int getX() const;
		unsigned int getY() const;
		movimientos getMov() const;
		State * operator =(const State * e);
};
#endif

