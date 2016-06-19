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
	cantidadDeMovimientos = 0;
	movimientosMax = 5;
}

ServerAvionEnemyGrande::~ServerAvionEnemyGrande() {
	delete t;
	delete posicion;
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


} /* namespace std */
