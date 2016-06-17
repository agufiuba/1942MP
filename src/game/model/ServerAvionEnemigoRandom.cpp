/*
 * ServerAvionEnemigoRandom.cpp
 *
 *  Created on: 16 de jun. de 2016
 *      Author: keynaka
 */

#include "ServerAvionEnemigoRandom.h"

ServerAvionEnemigoRandom::ServerAvionEnemigoRandom(Posicion* posicionInicial, Avion* avionApuntado) {

	this->id = id;
	this->vida = 1;

	anchoVista = 70; //TODO: hardcodeado por ahora, porque hay que conseguirlo desde el cliente
	largoVista = 60;

	viviendo = true;

	this->posicion = posicionInicial;

	anchoFondo = 1024; //TODO: hardcodeado por ahora, porque hay que conseguirlo desde el cliente
	largoFondo = 700;

	angleX = 0;
	angleY = -1;

	t = new Timer();

	firsttime = true;
	time_t tp = time(0);
	tm = localtime(&tp);
	nFlota = chrono::system_clock::now();

	this->avionApuntado = avionApuntado;
	contador = 0;
	tiempoEntreDisparo = 20;
}

ServerAvionEnemigoRandom::~ServerAvionEnemigoRandom(){
	delete t;
	delete posicion;
}

void ServerAvionEnemigoRandom::mover() {
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

