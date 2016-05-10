/*
 * Resolucion.cpp
 *
 *  Created on: 29/04/2016
 *      Author: gonzalo
 */

#include "Resolucion.h"
#include <X11/Xlib.h>

Resolucion* instancia = NULL;

Resolucion* Resolucion::INSTANCE() {
	if (!instancia)
		instancia = new Resolucion();
	return instancia;
}

Resolucion* Resolucion::INSTANCE(int x, int y) {
	if (!instancia)
		instancia = new Resolucion(x, y);
	return instancia;
}

Resolucion::Resolucion() {
	Display* disp = XOpenDisplay(NULL);
	Screen*  screen = DefaultScreenOfDisplay(disp);
	SCREEN_HEIGHT = screen->height;
	SCREEN_WIDTH  = screen->width;
}

Resolucion::Resolucion(int x, int y) {
	SCREEN_WIDTH  = x;
	SCREEN_HEIGHT = y;
}

int Resolucion::getWidthScreen() {
	return SCREEN_WIDTH;
}

int Resolucion::getHeightScreen() {
	return SCREEN_HEIGHT;
}

Resolucion::~Resolucion() {}

