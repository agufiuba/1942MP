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
#include "../../libs/data/game_data.h"

class HandlerPlayersControllers {

private:
	SDL_Renderer* renderer;
	Resolucion* resolucion;
	GameData* gameData;

public:
	map<string, IController*> mapaControllers;
	HandlerPlayersControllers(SDL_Renderer* renderer, Resolucion* resolucion, GameData* gameData);
	virtual ~HandlerPlayersControllers();
	void setPlayer(Avion* vivible);
	void mover(string id, char event);
	void hacerVivir();
	void hacerVueltereta();
	void killPlayer( string playerID );
	map<string, IController*> getMapaControllers();
};

#endif /* HANDLERPLAYERSCONTROLLERS_H_ */
