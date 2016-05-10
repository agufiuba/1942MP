/*
 * HandlerPlayersControllers.h
 *
 *  Created on: 07/05/2016
 *      Author: gonzalo
 */

#ifndef HANDLERPLAYERSCONTROLLERS_H_
#define HANDLERPLAYERSCONTROLLERS_H_

#include "SDL2/SDL.h"
#include <map>
#include "IController.h"
#include "PlayersController.h"
#include "../model/Avion.h"

class HandlerPlayersControllers {

private:
	map<string, IController*> mapaControllers;
	SDL_Renderer* renderer;
	Resolucion* resolucion;

public:
	HandlerPlayersControllers(SDL_Renderer* &renderer, Resolucion* resolucion);
	virtual ~HandlerPlayersControllers();
	void setPlayer(Avion* vivible);
	void mover(string id, char event);
	void hacerVivir();
};

#endif /* HANDLERPLAYERSCONTROLLERS_H_ */
