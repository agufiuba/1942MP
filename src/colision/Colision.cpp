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

bool Colision::is(Misil* misil, Enemy* otroVivible) {

	Vivible* unVivible = (Vivible*)misil;
	bool norteBool = true;

	if (misil->getVelocidadDisparo() <= 90) {
		norteBool = norte(unVivible, otroVivible);
	}

	return 	norteBool &&
			sur  (unVivible, otroVivible) &&
			oeste(unVivible, otroVivible) &&
			este (unVivible, otroVivible);
}

void Colision::view(Vivible* unVivible, Vivible* otroVivible, Screen* screen) {
	Posicion* posicionChispazo = new Posicion(unVivible->getX(), otroVivible->getY() - otroVivible->getLargo());
	ChispazoView* chispazo = new ChispazoView("chispazo", screen, posicionChispazo);
	delete chispazo;
	delete posicionChispazo;
}


void Colision::view(Misil* unVivible, Enemy* otroVivible, Screen* screen) {
	Posicion* posicionChispazo = new Posicion(unVivible->getX(), otroVivible->getY() - otroVivible->getLargo());
	ChispazoView* chispazo = new ChispazoView("chispazo", screen, posicionChispazo);
	delete chispazo;
	delete posicionChispazo;
}

// TODO: NO borrar por el momento

//	cout << "Velocidad misil" << misil->getVelocidadDisparo() << endl;
//	if (otroVivible->getLargo() == 110) {
//
//		cout << "Misil" << endl;
//		cout <<unVivible->getX() << "|" << unVivible->getY() << " - " << unVivible->getX() + unVivible->getAncho() << "|" << unVivible->getY() << endl;
//		cout <<unVivible->getX() << "|" << unVivible->getY() - unVivible->getLargo() << " - " << unVivible->getX() + unVivible->getAncho() << "|" << unVivible->getY() - unVivible->getLargo() << endl;
//		cout << "enemigo" << endl;
//		cout <<otroVivible->getX() << "|" << otroVivible->getY() << " - " << otroVivible->getX() + otroVivible->getAncho() << "|" << otroVivible->getY() << endl;
//		cout <<otroVivible->getX() << "|" << otroVivible->getY() - otroVivible->getLargo() << " - " << otroVivible->getX() + otroVivible->getAncho() << "|" << otroVivible->getY() - otroVivible->getLargo() << endl;
//	}

// 	int yMisilAux = misil->getY();
//
//	while (yMisilAux < 700 && !norteBool) {
//		cout << "Probando " << yMisilAux << endl;
//		norteBool = (otroVivible->getY() - otroVivible->getLargo()) - yMisilAux <= 0;
//		yMisilAux+=20;
//	}
//
//	norteBool = (norteBool && misil->getY() > otroVivible->getY());
//	cout << "Y " << misil->getY() << " - " << otroVivible->getY() << endl;
