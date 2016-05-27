/*
 * HandlerPowerUp.h
 *
 *  Created on: 25 de may. de 2016
 *      Author: keynaka
 */

#ifndef SRC_GAME_CONTROLLER_HANDLERPOWERUP_H_
#define SRC_GAME_CONTROLLER_HANDLERPOWERUP_H_

#include "SDL2/SDL.h"
#include <map>
#include "../model/PowerUp.h"

class HandlerPowerUp {
private:
	map<string, PowerUp*> mapaPowerUp;
	SDL_Renderer* renderer;
	Resolucion* resolucion;

public:
	HandlerPowerUp(SDL_Renderer* renderer, Resolucion* resolucion);
	virtual ~HandlerPowerUp();

	void activar(string id);
	void setPowerUp(PowerUp* powerUp);
	void hacerVivir();
};

#endif /* SRC_GAME_CONTROLLER_HANDLERPOWERUP_H_ */
