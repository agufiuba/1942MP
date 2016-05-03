/*
 * Misil.cpp
 *
 *  Created on: 30 de abr. de 2016
 *      Author: ramon
 */

#include "Misil.h"
using namespace std;

Misil::Misil(SDL_Renderer* renderer, int x, int y) {
	vistaMisil = new MisilView(renderer);

	viviendo = true;

	this->x = x;
	this->y = y;

	anchoFondo = 600; //TODO: hay que cambiarlo para que pueda conseguirlo desde el escenario
	largoFondo = 600;

	t = new Timer();

}

Misil::~Misil() {
	delete vistaMisil;
}

void Misil::vivir(int x,int velY){
	this->moverEjeY(velY);
	this->mostrar();
}

void Misil::mostrar(){
	vistaMisil->mostrar(this->x, this->y);
}

void Misil::moverEjeY(int velY){
	y -= velY;

	if ((y < 0) || (y + getLargo() > getLargoFondo())) {
		viviendo = false;
	}
}

int Misil::getAnchoFondo() {
	return anchoFondo;
}

int Misil::getLargoFondo() {
	return largoFondo;
}


bool Misil::aunVive(){
	return viviendo;
}
