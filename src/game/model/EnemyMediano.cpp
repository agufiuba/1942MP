/*
 * EnemyMediano.cpp
 *
 *  Created on: 18 de jun. de 2016
 *      Author: keynaka
 */

#include "EnemyMediano.h"

EnemyMediano::EnemyMediano(Screen* screen, SDL_Renderer * renderer, Resolucion* &resolucion, Posicion* posicionInicial, GameConf* conf)
:Enemy(screen, renderer, resolucion, posicionInicial, conf) {
		this->setHP(5);
		this->vistaAvion->cargarImagenDelAvion("AvionSprites_enemigo.bmp");
}

EnemyMediano::~EnemyMediano() {
	if (!explosion->exploto()) {
	   posicion->mover(-1, -3);
	   explosion->explotar(posicion);
	}

	if (vistaAvion != NULL) delete vistaAvion;
	if (explosion != NULL) delete explosion;
	delete t;
	delete posicion;
	delete controlDeMisiles;
}

