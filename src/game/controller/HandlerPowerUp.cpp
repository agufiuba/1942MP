/*
 * HandlerPowerUp.cpp
 *
 *  Created on: 25 de may. de 2016
 *      Author: keynaka
 */

#include "HandlerPowerUp.h"

HandlerPowerUp::HandlerPowerUp(SDL_Renderer* renderer, Resolucion* resolucion) {
	this->renderer = renderer;
	this->resolucion = resolucion;
}

HandlerPowerUp::~HandlerPowerUp() {
	for (map<string, PowerUp*>::iterator it = this->mapaPowerUp.begin(); it != this->mapaPowerUp.end(); ++it) {
		delete it->second;
	}
}

void HandlerPowerUp::setPowerUp(PowerUp* powerUp) {
	powerUp->setVelocidadStandard(1); //TODO: se debe cargar por XML
	this->mapaPowerUp[powerUp->getId()] = powerUp;
}

void HandlerPowerUp::activar(string id) {
	this->mapaPowerUp[id]->activarPowerUp();
}

void HandlerPowerUp::hacerVivir() {
	//mutex theMutex;
	//theMutex.lock();
	for (map<string, PowerUp*>::iterator it = this->mapaPowerUp.begin(); it != this->mapaPowerUp.end(); ++it) {
		if(it->second->aunVive()){
			it->second->vivir();
		} else {
			delete it->second;
			this->mapaPowerUp.erase(it);
		}
	}
	//theMutex.unlock();
}

void HandlerPowerUp::matar(string id) {
	this->mapaPowerUp[id]->morir();
}
