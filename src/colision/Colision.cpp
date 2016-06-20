#include "Colision.h"

bool Colision::is(int x, int y, int xp, int yp, int x2, int y2, int xp2, int yp2) {
	return (inside(x, y, xp, yp, x2, y2) ||
			inside(x, y, xp, yp, x2, yp2) ||
			inside(x, y, xp, yp, xp2, y2) ||
			inside(x, y, xp, yp, xp2, yp2));
}

bool Colision::inside(int x, int y, int xp, int yp, int x2, int y2) {
//	cout<<"Enemigo: posX "<<x2<<" posXFinal "<<xp2<<" posY "<<y2
	return (x2 >= x) && (x2 <= xp) && (y2 >= y) && (y2 <= yp);
}


bool Colision::is(Vivible* unVivible, Vivible* otroVivible) {
	return 	norte(unVivible, otroVivible) &&
			sur  (unVivible, otroVivible) &&
			oeste(unVivible, otroVivible) &&
			este (unVivible, otroVivible);
}

bool Colision::norte(Vivible* unVivible, Vivible* otroVivible) {
	return ((otroVivible->getY() - otroVivible->getLargo()) - unVivible->getY() <= 0);
}

bool Colision::sur(Vivible* unVivible, Vivible* otroVivible) {
	return ((unVivible->getY() - unVivible->getLargo()) - otroVivible->getY() <= 0);
}

bool Colision::este(Vivible* unVivible, Vivible* otroVivible) {
	return (otroVivible->getX() - (unVivible->getX() + unVivible->getAncho()) <= 0);
}

bool Colision::oeste(Vivible* unVivible, Vivible* otroVivible) {
	return (unVivible->getX() - (otroVivible->getX() + otroVivible->getAncho()) <= 0);
}

bool Colision::is(Vivible* unVivible, Enemy* otroVivible) {

//	if (unVivible->getLargo() == 170) {
////		cout << "id: " <<unVivible->getId() << endl;
//
//		cout << "Avion grande" << endl;
//		cout <<unVivible->getX() << "|" << unVivible->getY() << " - " << unVivible->getX() + unVivible->getAncho() << "|" << unVivible->getY() << endl;
//		cout <<unVivible->getX() << "|" << unVivible->getY() - unVivible->getLargo() << " - " << unVivible->getX() + unVivible->getAncho() << "|" << unVivible->getY() - unVivible->getLargo() << endl;
//		cout << "Disparo" << endl;
//		cout <<otroVivible->getX() << "|" << otroVivible->getY() << " - " << otroVivible->getX() + otroVivible->getAncho() << "|" << otroVivible->getY() << endl;
//		cout <<otroVivible->getX() << "|" << otroVivible->getY() - otroVivible->getLargo() << " - " << otroVivible->getX() + otroVivible->getAncho() << "|" << otroVivible->getY() - otroVivible->getLargo() << endl;
//	}
//	cout << "Entro en is" << endl;
//	otroVivible->getX();
//	cout << "Pudo agarrar el X" << endl;

	return 	norte(unVivible, otroVivible) &&
			sur  (unVivible, otroVivible) &&
			oeste(unVivible, otroVivible) &&
			este (unVivible, otroVivible);
}

bool Colision::norte(Vivible* unVivible, Enemy* otroVivible) {
	return ((otroVivible->getY() - otroVivible->getLargo()) - unVivible->getY() <= 0);
}

bool Colision::sur(Vivible* unVivible, Enemy* otroVivible) {
	return ((unVivible->getY() - unVivible->getLargo()) - otroVivible->getY() <= 0);
}

bool Colision::este(Vivible* unVivible, Enemy* otroVivible) {
	return (otroVivible->getX() - (unVivible->getX() + unVivible->getAncho()) <= 0);
}

bool Colision::oeste(Vivible* unVivible, Enemy* otroVivible) {
	return (unVivible->getX() - (otroVivible->getX() + otroVivible->getAncho()) <= 0);
}
