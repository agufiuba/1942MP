/*
 * Misil.cpp
 *
 *  Created on: 30 de abr. de 2016
 *      Author: ramon
 */

#include "Misil.h"
using namespace std;

Misil::Misil(SDL_Renderer* renderer, Posicion* posicion, Resolucion* resolucion, MisilConf* config) {

	if (config != NULL) {
		vistaMisil = new MisilView(renderer, config);
	}

	sonido = new Sound("laser.wav");
	sonido->play();

	viviendo = true;

	this->posicion = posicion;

	anchoFondo = resolucion->getWidthScreen();
	largoFondo = resolucion->getHeightScreen();

	t = new Timer();
	dano = 1;

}

Misil::~Misil() {
	delete sonido;
	delete vistaMisil;
}

void Misil::vivir(int x,int velY){
	this->moverEjeY(velY);
	this->mostrar();
}

void Misil::mostrar(){
	vistaMisil->mostrar(this->posicion->getX(), this->posicion->getYsdl());
}

void Misil::moverEjeY(int velY){
	posicion->mover(0, velY);
	int y = posicion->getY();
	if (y + getLargo() > getLargoFondo()) {
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

int Misil::getDano() {
	return this->dano;
}

void Misil::setAmetralladora() {
	vistaMisil->cargarImagenDelMisil("balaAmetralladora.bmp");
}
