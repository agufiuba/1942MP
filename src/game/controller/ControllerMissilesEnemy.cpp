/*
 * ControllerMissilesEnemy.cpp
 *
 *  Created on: 15 de jun. de 2016
 *      Author: keynaka
 */

#include "ControllerMissilesEnemy.h"

using namespace std;

ControllerMissilesEnemy::ControllerMissilesEnemy(MisilConf* config, SDL_Renderer* renderer){
	rendererMisil = renderer;
	vivibles = new CompositeVivibles();
	ametralladora = false;

	this->distanciaDeDesplazamiento = config->velocidadDisparos;
}

ControllerMissilesEnemy::~ControllerMissilesEnemy(){
	delete vivibles;
}

void ControllerMissilesEnemy::press(SDL_Event *event){
}

void ControllerMissilesEnemy::crearNuevoMisilEnPosicion(int x, int y,Resolucion* resolucion, MisilConf* config, int posicionApuntadoX, int posicionApuntadoY){
	//TODO:Para que el avion tire tiros dirigidos
	Vivible* misilNuevo = new MisilDirigido(rendererMisil, new Posicion(x,y),resolucion, config, posicionApuntadoX, posicionApuntadoY);

	if (ametralladora) misilNuevo->setAmetralladora();

	this->vivibles->agregarObjetoVivible(misilNuevo);
}

void ControllerMissilesEnemy::hacerVivir(){
		this->vivibles->vivir(0,this->distanciaDeDesplazamiento);
}

void ControllerMissilesEnemy::setAmetralladora(){
	ametralladora = true;
}

void ControllerMissilesEnemy::setBalaNormal(){
	ametralladora = false;
}

CompositeVivibles* ControllerMissilesEnemy::getVivibles(){
	return this->vivibles;
}
