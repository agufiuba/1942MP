/*
 * ServerAvionEnemigoRandom.cpp
 *
 *  Created on: 16 de jun. de 2016
 *      Author: keynaka
 */

#include "ServerAvionEnemigoRandom.h"

ServerAvionEnemigoRandom::ServerAvionEnemigoRandom(Posicion* posicionInicial):ServerAvionEnemigo(Posicion* posicionInicial) {

}

ServerAvionEnemigoRandom::~ServerAvionEnemigoRandom(){
	delete t;
	delete posicion;
}

void ServerAvionEnemigoRandom::vivir() {
	int x, y;
	time_t temp = time(0);
	localtime(&temp);

	if ((tm->tm_sec - secsRandom) < 1 && !firsttime) {
		x = angleX;
		y = angleY;
	}
	else {
		srand(time(NULL));
		x = rand() % 11 - 5;
		y = rand() % 11 - 5;

		// centrar +/-
		x -= (posicion->getX() - anchoFondo / 2) / 120;
		y -= (posicion->getY() - largoFondo / 2) / 150;
		firsttime = false;
		angleX = x;
		angleY = y;
		secsRandom = tm->tm_sec;
	}

	moverEjeX(x);
	moverEjeY(y);
}

