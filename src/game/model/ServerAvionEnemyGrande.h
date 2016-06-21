/*
 * ServerAvionEnemyGrande.h
 *
 *  Created on: 18 de jun. de 2016
 *      Author: ramon
 */

#ifndef SRC_GAME_MODEL_SERVERAVIONENEMYGRANDE_H_
#define SRC_GAME_MODEL_SERVERAVIONENEMYGRANDE_H_

#include <iostream>
#include <ctime>
#include <chrono>
#include "../view/AvionView.h"
#include "Vivible.h"
#include "../controller/Timer.h"
#include "Resolucion.h"
#include "../model/Posicion.h"
#include "../../libs/data/player_data.h"
#include "../../xml/conf/AvionConf.h"
#include <string>
#include "Misil.h"
#include "../view/ExplosionView.h"
#include "../view/Screen.h"
#include "../controller/ControllerMissilesEnemy.h"
#include "../model/ServerAvionEnemigo.h"

namespace std {

class ServerAvionEnemyGrande :public ServerAvionEnemigo{
public:
	ServerAvionEnemyGrande( int id, Posicion* posicionInicial);
	~ServerAvionEnemyGrande();
	EnemyData* vivir();
	int getHitScore();
	int getKillScore();
	void hitBy(int);
	void bajarHP(int);
	int cfd;
	bool mismoJugador;
private:
	int cantidadDeMovimientos;
	int movimientosMax;
};

} /* namespace std */

#endif /* SRC_GAME_MODEL_SERVERAVIONENEMYGRANDE_H_ */
