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

	char type;
	int id;
	int anchoFondo, largoFondo;
	int vida;
	bool viviendo;
	bool active;
	char type;

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
	ServerAvionEnemigo( int id, Posicion* posicionInicial );
	~ServerAvionEnemigo();
	char getType();
	int getId();
	int getAnchoFondo();
	int getLargoFondo();
	int getAncho();
	int getLargo();
	int getX();
	int getY();
	int getVida();
	bool isActive();
	void activate();
	void updatePosition( int x, int y );

	virtual EnemyData* vivir();

	bool aunVive();
	void morir();

	bool tieneHP();
	void setHP(int hp);
	void recibirMisil(Misil* misil);
	void bajarHP();

	void disparar();
	virtual int getHitScore();
	virtual int getKillScore();

	Posicion* getPosicion();

	bool firsttime;
	struct tm* tm;
	int secsRandom;
	chrono::time_point<chrono::system_clock> nFlota;
};

#endif /* SRC_GAME_MODEL_SERVERAVIONENEMIGO_H_ */
