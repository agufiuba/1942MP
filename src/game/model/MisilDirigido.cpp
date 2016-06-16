/*
 * MisilDirigido.cpp
 *
 *  Created on: 14 de jun. de 2016
 *      Author: keynaka
 */

#include "MisilDirigido.h"

using namespace std;

MisilDirigido::MisilDirigido(SDL_Renderer* renderer, Posicion* posicion, Resolucion* resolucion, MisilConf* config, int posicionApuntadoX, int posicionApuntadoY) {

	if (config != NULL) {
		vistaMisil = new MisilView(renderer, config);
	}

	sonido = new Sound("laser.wav");
	sonido->play();

	viviendo = true;

	this->posicion = posicion;
	posX = posicion->getX();
	posY = posicion->getY();

	anchoFondo = resolucion->getWidthScreen();
	largoFondo = resolucion->getHeightScreen();

	t = new Timer();
	dano = 1;

	this->vectorVelocidad = this->apuntar(posicionApuntadoX, posicionApuntadoY, 10);

}

MisilDirigido::~MisilDirigido() {
	delete sonido;
	delete vistaMisil;
}

void MisilDirigido::vivir(int x,int velY){
	this->mover();
	this->mostrar();
}

void MisilDirigido::mostrar(){
	vistaMisil->mostrar(this->posicion->getX(), this->posicion->getYsdl());
}

void MisilDirigido::mover(){
	posicion->mover(vectorVelocidad->getX(), vectorVelocidad->getY());
	posY = posicion->getY();
	posX = posicion->getX();
	if (posY + getLargo() > getLargoFondo()) {
		viviendo = false;
	}
}

Posicion* MisilDirigido::apuntar(int posicionApuntadoX, int posicionApuntadoY, int velocidad) {
	int ejeX =  abs(posicionApuntadoX) - abs(this->posX);
	int ejeY = abs(posicionApuntadoY) - abs(this->posY);
	double velX;
	double velY;

	double proporcion;

	if (ejeX == 0) {
		velX = velocidad;
		velY = 0;
	} else if (ejeY == 0) {
		velX = 0;
		velY = velocidad;
	} else if (abs(ejeX) == abs(ejeY)) {
		velX = velocidad;
		velY = velocidad;
	} else if (abs(ejeX) > abs(ejeY)) {
		 proporcion = abs(ejeX) / abs(ejeY);
		 velX = velocidad;
		 velY = velocidad / proporcion;
	} else {
		 proporcion = abs(ejeY) / abs(ejeX);
		 velX = velocidad / proporcion;
		 velY = velocidad;
	}

	if (ejeX < 0) {
		velX = - velX;
	}
	if (ejeY < 0) {
		velY = - velY;
	}

	return new Posicion(velX, velY);
}

int MisilDirigido::getAnchoFondo() {
	return this->anchoFondo;
}

int MisilDirigido::getLargoFondo() {
	return this->largoFondo;
}


bool MisilDirigido::aunVive(){
	return this->viviendo;
}

int MisilDirigido::getDano() {
	return this->dano;
}

void MisilDirigido::setAmetralladora() {
	vistaMisil->cargarImagenDelMisil("balaAmetralladora.bmp");
}

int MisilDirigido::getAncho(){
	return this->vistaMisil->getAncho();
}

int MisilDirigido::getLargo(){
	return this->vistaMisil->getLargo();
}

void MisilDirigido::morir(){
	this->viviendo = false;
}
