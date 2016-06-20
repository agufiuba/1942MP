/*
 * FlotaObserver.cpp
 *
 *  Created on: 19 de jun. de 2016
 *      Author: keynaka
 */

#include "FlotaObserver.h"

FlotaObserver::FlotaObserver() {
	this->cfd = 0;
	this->todosMatadosPorUnJugador = true;
	this->cantidadDeAvionesEnLaFlota = 5;
	this->contador = 0;
}

FlotaObserver::~FlotaObserver() {
}

void FlotaObserver::actualizar(int cfdQueEliminoAlAvion) {

	//este if es para que si una vez ya se seteo en false, ya no analice mas
	if (this->todosMatadosPorUnJugador) {
		//este if es para el primer cfd que ingresa
		if (this->cfd == 0) {
			this->cfd = cfdQueEliminoAlAvion;
		} else {
			this->todosMatadosPorUnJugador = (cfdQueEliminoAlAvion == this->cfd);
		}
	}

	this->contador++;

	if ((this->cantidadDeAvionesEnLaFlota == this->contador) && this->todosMatadosPorUnJugador) {
		//TODO: Hay que avisar que se debe enviar el bonus
	}
}
