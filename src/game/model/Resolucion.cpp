/*
 * Resolucion.cpp
 *
 *  Created on: 29/04/2016
 *      Author: gonzalo
 */

#include "Resolucion.h"
#include <X11/Xlib.h>

Resolucion::Resolucion() {
	Display* disp = XOpenDisplay(NULL);
	Screen*  screen = DefaultScreenOfDisplay(disp);
	SCREEN_HEIGHT = screen->height;
	SCREEN_WIDTH  = screen->width;
}

Resolucion::Resolucion(int x, int y) {
	SCREEN_HEIGHT = x;
	SCREEN_WIDTH  = y;
}

int Resolucion::getWidthScreen() {
	return SCREEN_WIDTH;
}

int Resolucion::getHeightScreen() {
	return SCREEN_HEIGHT;
}

Resolucion::~Resolucion() {}

