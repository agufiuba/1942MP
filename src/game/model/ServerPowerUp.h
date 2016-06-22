/*
 * ServerPowerUp.h
 *
 *  Created on: 21 de jun. de 2016
 *      Author: keynaka
 */

#ifndef SRC_GAME_MODEL_SERVERPOWERUP_H_
#define SRC_GAME_MODEL_SERVERPOWERUP_H_

#include <iostream>
#include <string>
#include "Vivible.h"
#include "Resolucion.h"
#include "../model/Posicion.h"
#include "../../libs/data/player_data.h"
#include "../../libs/data/enemy_data.h"
#include "../view/Screen.h"

class ServerPowerUp {
private:

	int anchoFondo, largoFondo;
	int vida;
	bool viviendo;
	bool active;
	char type;

	int anchoVista;
	int largoVista;

	Posicion* posicion;

	void moverEjeX(int velX);
	void moverEjeY(int velY);

public:
	ServerPowerUp( char type, Posicion* posicionInicial );
	~ServerPowerUp();

	int getAnchoFondo();
	int getLargoFondo();
	int getAncho();
	int getLargo();

	char getType();

	int getX();
	int getY();

	bool isActive();
	void activate();
	void updatePosition( int x, int y );

	bool aunVive();
	void morir();

	bool tieneHP();
	void setHP(int hp);

	Posicion* getPosicion();
};

#endif /* SRC_GAME_MODEL_SERVERPOWERUP_H_ */
