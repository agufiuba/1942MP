/*
 * EnemyGrande.h
 *
 *  Created on: 18 de jun. de 2016
 *      Author: ramon
 */

#ifndef SRC_GAME_MODEL_ENEMYGRANDE_H_
#define SRC_GAME_MODEL_ENEMYGRANDE_H_

#include "Enemy.h"

class EnemyGrande : public Enemy {
public:
	EnemyGrande(Screen* screen, SDL_Renderer * renderer, Resolucion* &resolucion, Posicion* posicionInicial, GameConf* conf);
	~EnemyGrande();
};

#endif /* SRC_GAME_MODEL_ENEMYGRANDE_H_ */
