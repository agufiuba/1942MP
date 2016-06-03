#include "Colision.h"

bool Colision::is(int x, int y, int xp, int yp, int x2, int y2, int xp2, int yp2) {
	return (inside(x, y, xp, yp, x2, y2) ||
			inside(x, y, xp, yp, x2, yp2) ||
			inside(x, y, xp, yp, xp2, y2) ||
			inside(x, y, xp, yp, xp2, yp2));
}

bool Colision::inside(int x, int y, int xp, int yp, int x2, int y2) {
	return (x2 >= x) && (x2 <= xp) && (y2 >= y) && (y2 <= yp);
}
