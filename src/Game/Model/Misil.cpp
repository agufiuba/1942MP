/*
 * Misil.cpp
 *
 *  Created on: 30 de abr. de 2016
 *      Author: ramon
 */

#include "Misil.h"
using namespace std;

Misil::Misil(SDL_Renderer * &renderer, int x, int y) {
	vistaMisil = new MisilView(renderer);

	this->x = x;
	this->y = y;

	anchoFondo = 600; //TODO: hay que cambiarlo para que pueda conseguirlo desde el escenario
	largoFondo = 600;

	t = new Timer();

}

Misil::~Misil() {
	// TODO Auto-generated destructor stub
}

void Misil::mostrar(){

}
void Misil::mover(int velX, int velY){

}

void Misil::moverEjeX(int velX){

}

void Misil::moverEjeY(int velY){

}
