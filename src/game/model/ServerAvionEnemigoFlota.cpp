/*
 * ServerAvionEnemigoFlota.cpp
 *
 *  Created on: 18 de jun. de 2016
 *      Author: keynaka
 */

#include "ServerAvionEnemigoFlota.h"


ServerAvionEnemigoFlota::ServerAvionEnemigoFlota( int id, Posicion* posicionInicial, int posicionEnLaFlota)
:ServerAvionEnemigo(id, posicionInicial) {
	esquinaDerecha = false;
	this->tiempoDeEspera = posicionEnLaFlota;
	this->type = 'f';
}

ServerAvionEnemigoFlota::~ServerAvionEnemigoFlota(){
	delete t;
	delete posicion;
}

EnemyData* ServerAvionEnemigoFlota::vivir() {
		enum Direction {
			U, D, R, L
		};
		Direction d;
		EnemyData* ed = new EnemyData;
		ed->id = this->id;

	if (this->tiempoDeEspera == 0) {

		int tiempoIda = 3200;
		int tiempoMuerto = 2250;
		int tiempoDespuesDeLaVuelta = 8000;

		if (!t->is_started()) {
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

		} else {
			if (t->tiempoActual() < tiempoIda + tiempoMuerto / 2) {
				ed->direction = 'U';

			} else {
				if (t->tiempoActual() < tiempoIda + tiempoMuerto) {
					if (esquinaDerecha) {
						ed->direction = 'R';
					} else {
						ed->direction = 'L';
					}

				} else {
					if (t->tiempoActual() < tiempoIda + tiempoMuerto + tiempoMuerto / 2) {
						ed->direction = 'D';

					} else {
						if (t->tiempoActual() < tiempoIda + tiempoMuerto + tiempoMuerto) {
							if (esquinaDerecha) {
								ed->direction = 'L';
							} else {
								ed->direction = 'R';
							}

						} else {
							if (t->tiempoActual()
									< tiempoIda + tiempoMuerto + tiempoMuerto
											+ tiempoDespuesDeLaVuelta) {
								if (esquinaDerecha) {
									ed->direction = 'Y'; // hacer que vaya solo a la izquierda
								} else {
									ed->direction = 'K'; //hacer que vaya solo a la derecha
								}

							} else {
								if (t->tiempoActual()
										>= tiempoIda + tiempoMuerto + tiempoMuerto
												+ tiempoDespuesDeLaVuelta) {
									ed->direction = 'Z'; //hacer que muera
								}
							}
						}
					}
				}
			}
		}
	} else {
		ed->direction = 'N';
		this->tiempoDeEspera --;
	}
		return ed;
}

void ServerAvionEnemigoFlota::addObserver(FlotaObserver* obs) {
	this->observer = obs;
}

void ServerAvionEnemigoFlota::notify(int cfd) {
	this->observer->actualizar(cfd);
}

void ServerAvionEnemigoFlota::bajarHP(int cfd) {
  if ( this->tieneHP() ) {
    this->vida--;
    cout << "ENEMY " << to_string( this->id ) << " HP = " << this->vida << endl;
  } else {
    this->viviendo = false;
  }

  this->notify(cfd);
}
