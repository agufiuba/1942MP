#include "Colision.h"

/**
 * unVivible debe ser el vivible mas pequeño,
 * debido a que verifica si unVivible se encuentra dentro de otroVivible
 */
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
