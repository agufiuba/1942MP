#ifndef COLISION_H
#define COLISION_H

class Colision {
public:
	static bool is(int, int, int, int, int, int, int, int);

private:
	static bool inside(int, int, int, int, int, int);
};

#endif
