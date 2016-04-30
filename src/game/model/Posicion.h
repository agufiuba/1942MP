/*
 * Posicion.h
 *
 *  Created on: 29/04/2016
 *      Author: gonzalo
 */

#ifndef POSICION_H_
#define POSICION_H_

class Posicion {

private:
	int x;
	int y;
	int limiteX;
	int limiteY;
	Resolucion* resolucion;
	void initResolucion();

public:
	Posicion(int x, int y);
	~Posicion();
	void setPosicion(int x, int y);
	int getX();
	int getY();
};

#endif /* POSICION_H_ */
