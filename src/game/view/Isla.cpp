/*
 * Isla.cpp
 *
 *  Created on: 03/05/2016
 *      Author: gonzalo
 */

#include "Isla.h"

using namespace std;

Isla::Isla(string index, Posicion* posicion, SpriteConf* sprite, XM_SDL* sdl) {

	this->id = sprite->path + index;
	this->posicion = posicion;
	string str(sprite->path);
	string path = "islas/"+str;
	islaTxt = new Texture( sdl->getRenderer() );
	islaTxt->loadFromFile(path);
//	islaTxt->render( posicion->getX(), posicion->getYsdl() );
//	screenIsla = new Screen(this->sdl);
//	screenIsla->loadTexture(index, path);
//	screenIsla->renderTexture(index, posicion->getX(), posicion->getYsdl());
}

Isla::~Isla() {
	delete posicion;
	delete islaTxt;
//	delete screenIsla;
}

void Isla::setPosicion(Posicion* posicion) {
	delete this->posicion;
	this->posicion = posicion;
}

void Isla::vivir() {
	posicion->mover(0, -3);
	islaTxt->render( posicion->getX(), posicion->getYsdl() );
//	screenIsla->renderTexture(id, posicion->getX(), posicion->getYsdl());
}

void Isla::vivir(int x, int y) {
	posicion->mover(x,-y);
	islaTxt->render( posicion->getX(), posicion->getYsdl() );
//	screenIsla->renderTexture(id, posicion->getX(), posicion->getYsdl());
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
