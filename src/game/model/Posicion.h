/*
 * Posicion.h
 *
 *  Created on: 29/04/2016
 *      Author: gonzalo
 */

#ifndef POSICION_H_
#define POSICION_H_

#include "Resolucion.h"

class Posicion {

private:
	int x;
	int y; //Posicion SDL
	int limiteY;
	Resolucion* resolucion;
	void initResolucion();

public:
	Posicion(int x, int y);
	~Posicion();
	void setPosicion(int x, int y);
	int getX();
	int getY();
	int getYsdl();
	void mover(int x, int y);
	void print();
};

#endif /* POSICION_H_ */
