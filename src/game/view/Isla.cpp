/*
 * Isla.cpp
 *
 *  Created on: 03/05/2016
 *      Author: gonzalo
 */

#include "Isla.h"

using namespace std;

Isla::Isla(string index, Posicion* posicion, SpriteConf* sprite, Screen* screenIsla) {

	this->id = sprite->path + index;
	this->posicion = posicion;
	string str(sprite->path);
	string path = "islas/"+str;
	this->screenIsla = screenIsla;
	this->screenIsla->loadTexture(id, path);
}

Isla::~Isla() {
	delete posicion;
}

void Isla::setPosicion(Posicion* posicion) {
	delete this->posicion;
	this->posicion = posicion;
}

void Isla::vivir() {
	posicion->mover(0, -3);
	screenIsla->renderTexture(id, posicion->getX(), posicion->getYsdl());
}

void Isla::vivir(int x, int y) {
	posicion->mover(x,-y);
	screenIsla->renderTexture(id, posicion->getX(), posicion->getYsdl());
}

string Isla::getId() {return this->id;}
int Isla::getAnchoFondo() {return 0;}
int Isla::getLargoFondo(){return 0;}
int Isla::getAncho(){return 0;}
int Isla::getLargo(){return 0;}
int Isla::getX(){return 0;}
int Isla::getY(){return 0;}

void Isla::inicializoVueltereta(){}

bool Isla::aunVive() {return true;}

Posicion* Isla::getPosicion() {
	return posicion;
}
