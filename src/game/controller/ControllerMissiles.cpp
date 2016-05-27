/*
 * ControllerAutomatic.cpp
 *
 *  Created on: 30 de abr. de 2016
 *      Author: ramon
 */

#include "ControllerMissiles.h"

using namespace std;

ControllerMissiles::ControllerMissiles(MisilConf* config, SDL_Renderer* renderer){
	rendererMisil = renderer;
	vivibles = new CompositeVivibles();
	ametralladora = false;

	this->distanciaDeDesplazamiento = config->velocidadDisparos;
}

ControllerMissiles::~ControllerMissiles(){
	delete vivibles;
}

void ControllerMissiles::press(SDL_Event *event){
}

void ControllerMissiles::crearNuevoMisilEnPosicion(int x, int y,Resolucion* resolucion, MisilConf* config){
	Vivible* misilNuevo = new Misil(rendererMisil, new Posicion(x,y),resolucion, config);

	if (ametralladora) misilNuevo->setAmetralladora();

	this->vivibles->agregarObjetoVivible(misilNuevo);
}

void ControllerMissiles::hacerVivir(){
		this->vivibles->vivir(0,this->distanciaDeDesplazamiento);
}

void ControllerMissiles::setAmetralladora(){
	ametralladora = true;
}

void ControllerMissiles::setBalaNormal(){
	ametralladora = false;
}
