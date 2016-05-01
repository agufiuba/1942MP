/*
 * Posicion.cpp
 *
 *  Created on: 29/04/2016
 *      Author: gonzalo
 */

#include "Posicion.h"
#include <iostream>


Posicion::Posicion(int x, int y) {
	initResolucion();
	setPosicion(x, y);
}

Posicion::~Posicion() {
	resolucion->~Resolucion();
}

void Posicion::initResolucion() {
	resolucion = new Resolucion();
	limiteY = resolucion->getHeightScreen();
}

void Posicion::setPosicion(int x, int y) {
	this->x = x;
	this->y = (limiteY - y);
}

int Posicion::getX() {
	return x;
}

int Posicion::getY() {
	return (limiteY - y);
}

int Posicion::getYsdl() {
	return y;
}

void Posicion::mover(int x, int y) {
	this->x += x;
	this->y -= y;
}

void Posicion::print() {
	std::cout << "(" << getX() << "," << getY() << ")" << std::endl;
}


