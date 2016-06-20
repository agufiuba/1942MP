/*
 * ServerAvionEnemigoFlota.h
 *
 *  Created on: 18 de jun. de 2016
 *      Author: keynaka
 */

#ifndef SRC_GAME_MODEL_SERVERAVIONENEMIGOFLOTA_H_
#define SRC_GAME_MODEL_SERVERAVIONENEMIGOFLOTA_H_

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
#include "../model/FlotaObserver.h"

class ServerAvionEnemigoFlota :public ServerAvionEnemigo{
private:
	bool esquinaDerecha;
	int tiempoDeEspera;
	FlotaObserver* observer;
	int numeroDeFlota;

public:
	ServerAvionEnemigoFlota( int id, Posicion* posicionInicial, int posicionEnLaFlota, int numeroDeFlota);
	~ServerAvionEnemigoFlota();
	EnemyData* vivir();
	void addObserver(FlotaObserver* obs);
	void notify(int cfd);
	void bajarHP(int cfd);
	int getNumeroDeFlota();
	int getHitScore();
	int getKillScore();
};

#endif /* SRC_GAME_MODEL_SERVERAVIONENEMIGOFLOTA_H_ */
