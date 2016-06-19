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

	this->velocidadStandard = 5;
	this->velX = 0;
	this->velY = 0;
}

HandlerEnemigos::~HandlerEnemigos() {
	for (map<int, Enemy*>::iterator it = this->mapaEnemigos.begin(); it != this->mapaEnemigos.end(); ++it) {
		delete it->second;
	}
}

void HandlerEnemigos::createEnemigo(int id, char type, int posX, int posY) {
		Posicion* p = new Posicion(posX, posY);
		if (type == 'r'){
			Enemy* enemy = new Enemy(screen, renderer, resolucion, p, gc);
			mapaEnemigos[id] = enemy;
		}else if (type == 'f') {

		}else if (type == 'm') {
			Enemy* mediano = new EnemyMediano(screen, renderer, resolucion, p, gc);
			mapaEnemigos[id] = mediano;
		}else if (type == 'g') {
			cout<<"Enemigo Grande: "<<id<<endl;
			Enemy* grande = new EnemyGrande(screen, renderer, resolucion, p, gc);
			mapaEnemigos[id] = grande;
		}
}

void HandlerEnemigos::hacerVivir() {
	mutex theMutex;
	theMutex.lock();
	for (map<int, Enemy*>::iterator it = this->mapaEnemigos.begin(); it != this->mapaEnemigos.end(); ++it) {
		if(it->second->aunVive()){
			it->second->vivir();
		} else {
			delete it->second;
			this->mapaEnemigos.erase(it);
		}
	}
	theMutex.unlock();
}

void HandlerEnemigos::matar(int id) {
	this->mapaEnemigos[id]->morir();
}

void HandlerEnemigos::mover(int id, char evento) {
	map<int, Enemy*>::iterator it = this->mapaEnemigos.find( id );
	if ( it == this->mapaEnemigos.end() ) return;

	Enemy* enemigo = mapaEnemigos[id];

		switch(evento) {
			case 'R': if (enemigo->getVelX() <= 0){
					    enemigo->setVelX(velocidadStandard);
			  	      }
					  break; 		//Derecha
			case 'L': if (enemigo->getVelX() >= 0){
					  	enemigo->setVelX(-velocidadStandard);
					  }
					  break; 		//Izquierda
			case 'U': if (enemigo->getVelY() <= 0){
					    enemigo->setVelY(velocidadStandard);
					  }
					  break; 		//Arriba
			case 'D': if (enemigo->getVelY() >= 0){
						enemigo->setVelY(-velocidadStandard);
					  }
					  break; 		//Abajo

			case 'S': enemigo->disparar()	; break; 								//Disparar

			case 'N': enemigo->setVelX(0);
					  enemigo->setVelY(0);
					  break;

			case 'X': enemigo->morir();
								this->matar(id);
								break;									//Morir

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

void HandlerEnemigos::setAvionApuntar(int idEnemigo, string idAvion) {
	map<int, Enemy*>::iterator it = this->mapaEnemigos.find( idEnemigo );
	if ( it == this->mapaEnemigos.end() ) return;
	map<string, Avion*>::iterator it2 = this->mapaAvionesApuntables.find( idAvion );
	if ( it2 == this->mapaAvionesApuntables.end() ) return;

	Avion* avion = this->mapaAvionesApuntables[idAvion];
	this->mapaEnemigos[idEnemigo]->setAvionApuntado(avion);
}

void HandlerEnemigos::addAvionesApuntables(Avion* avion) {
	this->mapaAvionesApuntables[avion->getId()] = avion;
}

