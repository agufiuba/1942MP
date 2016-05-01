/*
 * ControllerAutomatic.cpp
 *
 *  Created on: 30 de abr. de 2016
 *      Author: ramon
 */

#include "ControllerMissiles.h"

using namespace std;

ControllerMissiles::ControllerMissiles(Avion* &unAvion, SDL_Renderer* &renderer){
	rendererMisil = renderer;
	vivibles = new CompositeVivibles();
	avion = unAvion;
}

ControllerMissiles::~ControllerMissiles() {
	Controller::~Controller();
}

void ControllerMissiles::press(SDL_Event *event){

	if( event->type == SDL_KEYDOWN && event->key.repeat == 0){
  	if (event->key.keysym.sym == SDLK_SPACE) {
  					Vivible* misilNuevo = new Misil(renderer,0,0);
  					this->vivibles->agregarObjetoVivible(misilNuevo);
  	}
  }
  else if( event->type == SDL_KEYUP && event->key.repeat == 0){
  	if (event->key.keysym.sym == SDLK_SPACE) {
  					Vivible* misilNuevo = new Misil(renderer,,0);
  					this->vivibles->agregarObjetoVivible(misilNuevo);
  	}
  }
}


void ControllerMissiles::hacerVivir(){
	obj->vivir(velX, velY);
}
