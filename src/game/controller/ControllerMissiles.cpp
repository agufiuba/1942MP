/*
 * ControllerAutomatic.cpp
 *
 *  Created on: 30 de abr. de 2016
 *      Author: ramon
 */

#include "ControllerMissiles.h"

using namespace std;

ControllerMissiles::ControllerMissiles(SDL_Renderer* &renderer){
	rendererMisil = renderer;
	vivibles = new CompositeVivibles();
}

ControllerMissiles::~ControllerMissiles(){
	delete vivibles;
}

void ControllerMissiles::press(SDL_Event *event){
}

void ControllerMissiles::crearNuevoMisilEnPosicion(int x, int y,Resolucion* resolucion){

	Vivible* misilNuevo = new Misil(rendererMisil, new Posicion(x,y),resolucion);
	this->vivibles->agregarObjetoVivible(misilNuevo);
}

void ControllerMissiles::hacerVivir(){
		this->vivibles->vivir(0,this->distanciaDeDesplazamiento);
}
