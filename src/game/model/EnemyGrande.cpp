/*
 * EnemyGrande.cpp
 *
 *  Created on: 18 de jun. de 2016
 *      Author: ramon
 */

#include "EnemyGrande.h"

EnemyGrande::EnemyGrande(Screen* screen, SDL_Renderer * renderer, Resolucion* &resolucion, Posicion* posicionInicial, GameConf* conf)
:Enemy(screen, renderer, resolucion, posicionInicial, conf) {
		this->setHP(10);
		this->vistaAvion->setAnchoVista(250);
		this->vistaAvion->setLargoVista(170);
		this->vistaAvion->cargarClips();
		this->vistaAvion->cargarImagenDelAvion("AvionSprites_grande.bmp");
}

EnemyGrande::~EnemyGrande() {
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
