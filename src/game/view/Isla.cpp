/*
 * Isla.cpp
 *
 *  Created on: 03/05/2016
 *      Author: gonzalo
 */

#include "Isla.h"

using namespace std;

Isla::Isla(SDL_Renderer* render, Posicion* posicion, string path) {
	this->posicion = posicion;
	islaTxt = new Texture( render );

	islaTxt->loadFromFile(path);
	islaTxt->render( posicion->getX(), posicion->getYsdl() );
}

Isla::~Isla() {
	delete posicion;
	delete islaTxt;
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
