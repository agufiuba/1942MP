/*
 * Isla.cpp
 *
 *  Created on: 03/05/2016
 *      Author: gonzalo
 */

#include "Isla.h"

Isla::Isla(Posicion* posicion, SDL_Renderer* render) {
	islaTxt = new Texture();
	this->posicion = posicion;
	this->render = render;
	islaTxt->loadFromFile(DIR_ISLA_1, render);
	islaTxt->render(posicion->getX(), posicion->getYsdl(), render, NULL);
}

Isla::~Isla() {

}

void Isla::vivir(int x, int y) {
	posicion->mover(0, -3);
	islaTxt->render(posicion->getX(), posicion->getYsdl(), render, NULL);
}

