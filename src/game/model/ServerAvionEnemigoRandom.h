/*
 * ServerAvionEnemigoRandom.h
 *
 *  Created on: 16 de jun. de 2016
 *      Author: keynaka
 */

#ifndef SRC_GAME_MODEL_SERVERAVIONENEMIGORANDOM_H_
#define SRC_GAME_MODEL_SERVERAVIONENEMIGORANDOM_H_

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

class ServerAvionEnemigoRandom :public ServerAvionEnemigo{

private:
  int randomSeed;
  int dAnt;
  void refreshSeed();
public:
	ServerAvionEnemigoRandom( int id, Posicion* posicionInicial);
	~ServerAvionEnemigoRandom();
	EnemyData* vivir();
};

#endif /* SRC_GAME_MODEL_SERVERAVIONENEMIGORANDOM_H_ */
