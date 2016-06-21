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
	posX = posicion->getX();
	posY = posicion->getY();

	anchoFondo = resolucion->getWidthScreen();
	largoFondo = resolucion->getHeightScreen();

	t = new Timer();
	dano = 1;
	velocidadDisparo = config->velocidadDisparos;
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
	posY = posicion->getY();
	if (posY + getLargo() > getLargoFondo()) {
		viviendo = false;
	}
}

int Misil::getAnchoFondo() {
	return this->anchoFondo;
}

int Misil::getLargoFondo() {
	return this->largoFondo;
}


bool Misil::aunVive(){
	return this->viviendo;
}

int Misil::getDano() {
	return this->dano;
}

void Misil::setAmetralladora() {
	vistaMisil->cargarImagenDelMisil("balaAmetralladora.bmp");
}

int Misil::getAncho(){
	return this->vistaMisil->getAncho();
}

int Misil::getLargo(){
	return this->vistaMisil->getLargo();
}

void Misil::morir(){
	this->viviendo = false;
}

int Misil::getX() {
	return this->posicion->getX();
}

int Misil::getY() {
	return this->posicion->getY();
}

int Misil::getVelocidadDisparo() {
	return this->velocidadDisparo;
}

Posicion* Misil::getPosicion() {
	return this->posicion;
}
