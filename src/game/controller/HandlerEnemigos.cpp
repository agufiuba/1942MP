/*
 * HandlerEnemigos.cpp
 *
 *  Created on: 17 de jun. de 2016
 *      Author: keynaka
 */

#include "HandlerEnemigos.h"

using namespace std;

HandlerEnemigos::HandlerEnemigos(SDL_Renderer* renderer, Resolucion* resolucion, Screen* screen, GameConf* gc) {
	this->renderer = renderer;
	this->resolucion = resolucion;
	this->screen = screen;
	this->gc = gc;

	this->velocidadStandard = 10;
	this->velX = 0;
	this->velY = 0;
}

HandlerEnemigos::~HandlerEnemigos() {
	for (map<int, Enemy*>::iterator it = this->mapaEnemigos.begin(); it != this->mapaEnemigos.end(); ++it) {
		delete it->second;
	}
}

void HandlerEnemigos::createEnemigo(int id, string type, int posX, int posY) {
		Posicion* p = new Posicion(posX, posY);
		Enemy* enemy = new Enemy(screen, renderer, resolucion, p, gc->avion);
		mapaEnemigos[id] = enemy;
}

void HandlerEnemigos::hacerVivir() {
	//mutex theMutex;
	//theMutex.lock();
	for (map<int, Enemy*>::iterator it = this->mapaEnemigos.begin(); it != this->mapaEnemigos.end(); ++it) {
		if(it->second->aunVive()){
			it->second->vivir(velX, velY);
		} else {
			delete it->second;
			this->mapaEnemigos.erase(it);
		}
	}
	//theMutex.unlock();
}

void HandlerEnemigos::matar(int id) {
	this->mapaEnemigos[id]->morir();
}

void HandlerEnemigos::mover(int id, char evento) {
	Enemy* enemigo = mapaEnemigos[id];

	if (enemigo != NULL) {
		switch(evento) {
			case 'R': if (velX <= 0){
									this->velX = velocidadStandard;
								}
									break; 		//Derecha
			case 'L': if (velX >= 0){
									this->velX = -velocidadStandard;
								}
									break; 		//Izquierda
			case 'U': if (velY <= 0){
									this->velY = velocidadStandard;
								}
									break; 		//Arriba
			case 'D': if (velY >= 0){
									this->velY = -velocidadStandard;
								}
									break; 		//Abajo

			case 'S': enemigo->disparar()	; break; 								//Disparar

			case 'X': enemigo->morir();
								this->matar(id);
								break;									//Morir

		}
	}
}

void HandlerEnemigos::deleteEnemys() {
	for (map<int, Enemy*>::iterator it = this->mapaEnemigos.begin(); it != this->mapaEnemigos.end(); ++it) {
		it->second->morir();
	}
}

Enemy* HandlerEnemigos::getEnemigo(int id) {
	return this->mapaEnemigos[id];
}
