/*
 * Isla.cpp
 *
 *  Created on: 03/05/2016
 *      Author: gonzalo
 */

#include "Isla.h"

Isla::Isla(SDL_Renderer* &render, Posicion* posicion, int islaNumber) {
	islaTxt = new Texture();
	this->posicion = posicion;
	this->render = render;

	vector<const char*> direcciones;
	direcciones.push_back(DIR_ISLA_1);
	direcciones.push_back(DIR_ISLA_2);
	direcciones.push_back(DIR_ISLA_3);
	direcciones.push_back(DIR_ISLA_4);
	direcciones.push_back(PORTAAVION);

	if (islaNumber >= 5) {
		islaNumber = 0;
	}
	islaTxt->loadFromFile(direcciones[islaNumber], render);


	islaTxt->render(posicion->getX(), posicion->getYsdl(), render, NULL);
}

Isla::~Isla() {

}

void Isla::vivir(int x, int y) {
	posicion->mover(0, -3);
	islaTxt->render(posicion->getX(), posicion->getYsdl(), render, NULL);

}

int Isla::getAnchoFondo() {return 0;}
int Isla::getLargoFondo(){return 0;}
int Isla::getAncho(){return 0;}
int Isla::getLargo(){return 0;}
int Isla::getX(){return 0;}
int Isla::getY(){return 0;}

void Isla::inicializoVueltereta(){}

bool Isla::aunVive() {return true;}

