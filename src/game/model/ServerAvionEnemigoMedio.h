#ifndef SRC_GAME_MODEL_SERVERAVIONENEMIGOMEDIO_H_
#define SRC_GAME_MODEL_SERVERAVIONENEMIGOMEDIO_H_

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

class ServerAvionEnemigoMedio :public ServerAvionEnemigo{

private:
  int randomSeed;
  int dAnt;
  void refreshSeed();
  void hitBy(int);
  int cfd;
public:
	void bajarHP(int);
  	bool mismoJugador;
	ServerAvionEnemigoMedio( int id, Posicion* posicionInicial);
	~ServerAvionEnemigoMedio();
	EnemyData* vivir();
	int getHitScore();
	int getKillScore();
};

#endif 
