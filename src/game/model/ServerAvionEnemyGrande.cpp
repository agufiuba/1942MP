/*
 * ServerAvionEnemyGrande.cpp
 *
 *  Created on: 18 de jun. de 2016
 *      Author: ramon
 */

#include "ServerAvionEnemyGrande.h"

namespace std {

ServerAvionEnemyGrande::ServerAvionEnemyGrande(int id, Posicion* posicionInicial
):ServerAvionEnemigo(id, posicionInicial) {
  this->type = 'g';
  this->vida = 10;
	cantidadDeMovimientos = 0;
	movimientosMax = 6;
	this->type = 'g';
	this->cfd = 0;
	this->mismoJugador = true;
}

ServerAvionEnemyGrande::~ServerAvionEnemyGrande() {
	delete t;
	delete posicion;
}

void ServerAvionEnemyGrande::hitBy(int cfdAvion) {
	if(cfd == 0) cfd = cfdAvion;
	mismoJugador = mismoJugador && (cfd == cfdAvion);
}

EnemyData* ServerAvionEnemyGrande::vivir() {
	enum Direction { U, D, R, L };
	Direction d;
	EnemyData* ed = new EnemyData;
	ed->id = this->id;

	if (this->cantidadDeMovimientos < this->movimientosMax){
		cout << "UP" << endl;
		ed->direction = 'U';
		this->cantidadDeMovimientos++;
	}else if(this->cantidadDeMovimientos == this->movimientosMax) {
		cout << "N" << endl;
		ed->direction = 'N';
		this->cantidadDeMovimientos++;
	} else {
		cout << "Z" << endl;
		ed->direction = 'Z';
	}
	return ed;
}

int ServerAvionEnemyGrande::getHitScore() {
  return 100;
}

int ServerAvionEnemyGrande::getKillScore() {
  return 1000;
}

void ServerAvionEnemyGrande::bajarHP(int cfdAvion) {
	if ( this->tieneHP() ) {
    this->vida--;
    cout << "ENEMY " << to_string( this->id ) << " HP = " << this->vida << endl;
  }
  if ( !this->tieneHP() ) {
    this->viviendo = false;
  }
	hitBy(cfdAvion);
}

} /* namespace std */
