/*
 * ServerAvionEnemigo.h
 *
 *  Created on: 17 de jun. de 2016
 *      Author: keynaka
 */

#ifndef SRC_GAME_MODEL_SERVERAVIONENEMIGO_H_
#define SRC_GAME_MODEL_SERVERAVIONENEMIGO_H_

#include <iostream>
#include <ctime>
#include <chrono>
#include "../view/AvionView.h"
#include "Vivible.h"
#include "../controller/Timer.h"
#include "Resolucion.h"
#include "../model/Posicion.h"
#include "../../libs/data/player_data.h"
#include "../../libs/data/enemy_data.h"
#include "../../xml/conf/AvionConf.h"
#include <string>
#include "Misil.h"
#include "../view/ExplosionView.h"
#include "../view/Screen.h"
#include "../controller/ControllerMissilesEnemy.h"

class ServerAvionEnemigo {
protected:

	string id;
	int anchoFondo, largoFondo;
	int vida;
	bool viviendo;

	int anchoVista;
	int largoVista;

	Timer* t;

	Posicion* posicion;

	int contador;
	int tiempoEntreDisparo;

	void moverEjeX(int velX);
	void moverEjeY(int velY);

	int angleX;
	int angleY;

	Avion* avionApuntado;

public:
	ServerAvionEnemigo(Posicion* posicionInicial);
	~ServerAvionEnemigo();
	string getId();
	int getAnchoFondo();
	int getLargoFondo();
	int getAncho();
	int getLargo();
	int getX();
	int getY();
	int getVida();

	virtual EnemyData* vivir();

	bool aunVive();
	void morir();

	bool tieneHP();
	void setHP(int hp);
	void recibirMisil(Misil* misil);

	void disparar();

	Posicion* getPosicion();

	bool firsttime;
	struct tm* tm;
	int secsRandom;
	chrono::time_point<chrono::system_clock> nFlota;
};

#endif /* SRC_GAME_MODEL_SERVERAVIONENEMIGO_H_ */
