/*
 * Resolucion.h
 *
 *  Created on: 29/04/2016
 *      Author: gonzalo
 */

#ifndef RESOLUCION_H_
#define RESOLUCION_H_

#include <iostream>

class Resolucion {

private:
	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;
	Resolucion();
	Resolucion(int x, int y);

public:
	static Resolucion* INSTANCE();
	static Resolucion* INSTANCE(int x, int y);
	~Resolucion();
	int getWidthScreen();
	int getHeightScreen();
	bool isFullScreen();

};

#endif /* RESOLUCION_H_ */
