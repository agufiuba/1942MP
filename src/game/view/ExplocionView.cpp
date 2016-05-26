/*
 * ExplocionView.cpp
 *
 *  Created on: 25/05/2016
 *      Author: gonzalo
 */

#include "ExplocionView.h"

ExplocionView::ExplocionView(string id, Screen* screen, Posicion* posicion) {

	this->id = id;
	this->screen = screen;
	string explocion = "explociones/explocion";
	string bmp = ".bmp";

	for (int i = 0; i < cantidadScreen; i++) {
		string pos = to_string(i/cantidadRepeticiones);
		string dir = explocion + pos + bmp;
		screens.push_back(dir);
	}

}

ExplocionView::~ExplocionView() {
//	TODO ver de borrar el texture
}

void ExplocionView::explotar(Posicion* posicion) {

	if (!exploto()) {
		string dir = screens[screenActual];
		this->screen->loadTexture(dir, dir);
		screen->renderTexture(dir, posicion->getX(), posicion->getYsdl());
		screenActual++;
	}
}

bool ExplocionView::exploto() {
	return screenActual >= cantidadScreen;
}
