/*
 * ServerAvionEnemigoFlota.cpp
 *
 *  Created on: 18 de jun. de 2016
 *      Author: keynaka
 */

#include "ServerAvionEnemigoFlota.h"


ServerAvionEnemigoFlota::ServerAvionEnemigoFlota( int id, Posicion* posicionInicial
):ServerAvionEnemigo(id, posicionInicial) {
	esquinaDerecha = false;
}

ServerAvionEnemigoFlota::~ServerAvionEnemigoFlota(){
	delete t;
	delete posicion;
}

EnemyData* ServerAvionEnemigoFlota::vivir() {
	enum Direction { U, D, R, L };
	Direction d;
	EnemyData* ed = new EnemyData;
	ed->id = this->id;

	int tiempoIda = 3200; //recomendado >> 1600
	int tiempoMuerto = 2200; //recomendado >> 400
	int tiempoDespuesDeLaVuelta = 8000;

	if (!t->is_started()) {
		cout << "inicio el timer" <<endl;
		if (this->getX() >= this->getAnchoFondo()) {
			esquinaDerecha = true;
		}
		t->correr();
	}

	if (t->tiempoActual() < tiempoIda) {
		if (esquinaDerecha) {
			ed->direction = 'L';
		} else {
			ed->direction = 'R';
		}
		cout << "1" <<endl;

	} else {
		if (t->tiempoActual() < tiempoIda + tiempoMuerto / 2) {
			ed->direction = 'U';
			cout << "2" <<endl;

		} else {
			if (t->tiempoActual() < tiempoIda + tiempoMuerto) {
				if (esquinaDerecha) {
					ed->direction = 'R';
				} else {
					ed->direction = 'L';
				}
				cout << "3" <<endl;

			} else {
				if (t->tiempoActual() < tiempoIda + tiempoMuerto + tiempoMuerto / 2) {
					ed->direction = 'D';
					cout << "4" <<endl;

				} else {
					if (t->tiempoActual() < tiempoIda + tiempoMuerto + tiempoMuerto) {
						if (esquinaDerecha) {
							ed->direction = 'L';
						} else {
							ed->direction = 'R';
						}
						cout << "5" <<endl;

					} else {
						if (t->tiempoActual() < tiempoIda + tiempoMuerto + tiempoMuerto + tiempoDespuesDeLaVuelta) {
							if (esquinaDerecha) {
								ed->direction = 'Y'; // hacer que vaya solo a la izquierda
							} else {
								ed->direction = 'K'; //hacer que vaya solo a la derecha
							}
							cout << "6" <<endl;

						} else {
							if (t->tiempoActual() >= tiempoIda + tiempoMuerto + tiempoMuerto + tiempoDespuesDeLaVuelta) {
								ed->direction = 'X'; //hacer que muera
								cout << "7" <<endl;
							}
						}
					}
				}
			}
		}
	}
	return ed;
}
