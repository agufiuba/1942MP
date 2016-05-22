/*
 * Isla.cpp
 *
 *  Created on: 03/05/2016
 *      Author: gonzalo
 */

#include "Isla.h"

using namespace std;

Isla::Isla(XM_SDL* sdl, Posicion* posicion, SpriteConf* sprite) {

	this->id = sprite->path;
	this->posicion = posicion;
//	islaTxt = new Texture( render );
	string str(sprite->path);
	string path = "islas/"+str;
//	islaTxt->loadFromFile(path);
//	islaTxt->render( posicion->getX(), posicion->getYsdl() );
	cout <<""
	screenIsla = new Screen(this->sdl);
	screenIsla->loadTexture(path, path);
	screenIsla->renderTexture(path, posicion->getX(), posicion->getYsdl());


}

Isla::~Isla() {
	delete posicion;
//	delete islaTxt;
	delete screenIsla;
}

void Isla::setPosicion(Posicion* posicion) {
	delete this->posicion;
	this->posicion = posicion;
}

void Isla::vivir() {
	posicion->mover(0, -3);
	//islaTxt->render( posicion->getX(), posicion->getYsdl() );
	screenIsla->renderTexture(path, posicion->getX(), posicion->getYsdl());
}

void Isla::vivir(int x, int y) {
	posicion->mover(x,-y);
	//islaTxt->render( posicion->getX(), posicion->getYsdl() );
	screenIsla->renderTexture(path, posicion->getX(), posicion->getYsdl());
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
