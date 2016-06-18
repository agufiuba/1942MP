/*
 * EnemyMediano.h
 *
 *  Created on: 18 de jun. de 2016
 *      Author: keynaka
 */

#ifndef SRC_GAME_MODEL_ENEMYMEDIANO_H_
#define SRC_GAME_MODEL_ENEMYMEDIANO_H_

#include "Enemy.h"

class EnemyMediano : public Enemy {
public:
	EnemyMediano(Screen* screen, SDL_Renderer * renderer, Resolucion* &resolucion, Posicion* posicionInicial, GameConf* conf);
	~EnemyMediano();
};

#endif /* SRC_GAME_MODEL_ENEMYMEDIANO_H_ */
