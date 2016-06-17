/*
 * HandlerEnemigos.cpp
 *
 *  Created on: 17 de jun. de 2016
 *      Author: keynaka
 */

#include "HandlerEnemigos.h"

using namespace std;

HandlerEnemigos::HandlerEnemigos(SDL_Renderer* renderer, Resolucion* resolucion, Screen* screen) {
	this->renderer = renderer;
	this->resolucion = resolucion;
	this->screen = screen;

	this->velocidadStandard = 10;
	this->velX = 0;
	this->velY = 0;
}

HandlerEnemigos::~HandlerEnemigos() {
	for (map<string, Vivible*>::iterator it = this->mapaEnemigos.begin(); it != this->mapaEnemigos.end(); ++it) {
		delete it->second;
	}
}

void HandlerEnemigos::createEnemigo(string id, string type, int posX, int posY) {
	//if (type == "random") {
		//(escenarioScreen, gRenderer, resolucion, p, gc->avion, (Avion*)myControl->getVivible());
		//Vivible* enemigo = new Enemy(screen, renderer, resolucion, new Posicion(posX, posY), , );
	//}
	//this->mapaEnemigos[id] = enemigo;
}

void HandlerEnemigos::hacerVivir() {
	//mutex theMutex;
	//theMutex.lock();
	for (map<string, Vivible*>::iterator it = this->mapaEnemigos.begin(); it != this->mapaEnemigos.end(); ++it) {
		if(it->second->aunVive()){
			it->second->vivir(velX, velY);
		} else {
			delete it->second;
			this->mapaEnemigos.erase(it);
		}
	}
	//theMutex.unlock();
}

void HandlerEnemigos::matar(string id) {
	this->mapaEnemigos[id]->morir();
}

void HandlerEnemigos::mover(string id, char evento) {

	Vivible* enemigo = mapaEnemigos[id];

	switch(evento) {
		case 'R': this->velX += velocidadStandard; break; 		//Derecha
		case 'L': this->velX -= velocidadStandard; break; 		//Izquierda
		case 'U': this->velY += velocidadStandard; break; 		//Arriba
		case 'D': this->velY -= velocidadStandard; break; 		//Abajo
		case 'S': enemigo->disparar()	; break; 								//Disparar

		case 'X': enemigo->morir();
							this->matar(id);
							break;									//Morir
	}
}

void HandlerEnemigos::deleteEnemys() {
	for (map<string, Vivible*>::iterator it = this->mapaEnemigos.begin(); it != this->mapaEnemigos.end(); ++it) {
		it->second->morir();
	}
}
