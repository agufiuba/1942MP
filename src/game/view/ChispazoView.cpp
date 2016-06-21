/*
 * ChispazoView.cpp
 *
 *  Created on: 20/06/2016
 *      Author: gvallerino
 */

#include "ChispazoView.h"

ChispazoView::ChispazoView(string id, Screen* screen, Posicion* posicion) {
	this->id = id;
	this->screen = screen;
	this->imagen = "explosiones/chispazo2.bmp";
	for (int i = 0; i < 50; i++) {
		this->screen->loadTexture(id, imagen);
		screen->renderTexture(id, posicion->getX(), posicion->getYsdl() - 30);
	}
}

ChispazoView::~ChispazoView() {
	// TODO Auto-generated destructor stub
}

