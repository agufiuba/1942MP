/*
 * Isla.cpp
 *
 *  Created on: 03/05/2016
 *      Author: gonzalo
 */

#include "Isla.h"

using namespace std;

Isla::Isla(SDL_Renderer* render, Posicion* posicion, string id) {
	this->posicion = posicion;
	islaTxt = new Texture( render );

	string dir = mapearIsla(id);

	islaTxt->loadFromFile(dir);
	islaTxt->render( posicion->getX(), posicion->getYsdl() );
}

Isla::~Isla() {
	delete posicion;
	delete islaTxt;
}

string Isla::mapearIsla(string id) {

	string tipoIsla = "src/game/images/image-not-found.bmp";

	if (id == "isla1")	tipoIsla = DIR_ISLA_1;
	else if (id == "isla2")	tipoIsla = DIR_ISLA_2;
	else if (id == "isla3")	tipoIsla = DIR_ISLA_3;
	else if (id == "isla4")	tipoIsla = DIR_ISLA_4;
	else if (id == "portaavion")	tipoIsla = PORTAAVION;

	return tipoIsla;
}

void Isla::vivir(int x, int y) {
	posicion->mover(0, -3);
	islaTxt->render( posicion->getX(), posicion->getYsdl() );

}

int Isla::getAnchoFondo() {return 0;}
int Isla::getLargoFondo(){return 0;}
int Isla::getAncho(){return 0;}
int Isla::getLargo(){return 0;}
int Isla::getX(){return 0;}
int Isla::getY(){return 0;}

void Isla::inicializoVueltereta(){}

bool Isla::aunVive() {return true;}

