/*
 * ExplosionView.cpp
 *
 *  Created on: 25/05/2016
 *      Author: gonzalo
 */

#include "ExplosionView.h"

ExplosionView::ExplosionView(string id, Screen* screen, Posicion* posicion) {

	this->id = id;
	this->screen = screen;
	string explosion = "explosiones/explosion";
	string bmp = ".bmp";

	for (int i = 0; i < cantidadScreen; i++) {
		string pos = to_string(i/cantidadRepeticiones);
		string dir = explosion + pos + bmp;
		screens.push_back(dir);
	}

}

ExplosionView::~ExplosionView() {
//	TODO ver de borrar el texture
}

void ExplosionView::explotar(Posicion* posicion) {

	if (!exploto()) {
		string dir = screens[screenActual];
		this->screen->loadTexture(dir, dir);
		screen->renderTexture(dir, posicion->getX(), posicion->getYsdl());
		screenActual++;
	}
}

bool ExplosionView::exploto() {
	return screenActual >= cantidadScreen;
}
