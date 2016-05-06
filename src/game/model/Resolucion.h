/*
 * Resolucion.h
 *
 *  Created on: 29/04/2016
 *      Author: gonzalo
 */

#ifndef RESOLUCION_H_
#define RESOLUCION_H_

class Resolucion {

private:
	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;

public:
	Resolucion();
	Resolucion(int x, int y);
	~Resolucion();
	int getWidthScreen();
	int getHeightScreen();

};

#endif /* RESOLUCION_H_ */
