/*
 * HandlerEnemigos.h
 *
 *  Created on: 17 de jun. de 2016
 *      Author: keynaka
 */

#ifndef SRC_GAME_CONTROLLER_HANDLERENEMIGOS_H_
#define SRC_GAME_CONTROLLER_HANDLERENEMIGOS_H_

#include "SDL2/SDL.h"
#include <map>
#include "IController.h"
#include "PlayersController.h"
#include "../model/Avion.h"
#include "../model/Vivible.h"
#include "../../libs/data/game_data.h"
#include "../view/Screen.h"

class HandlerEnemigos {

private:
	SDL_Renderer* renderer;
	Resolucion* resolucion;
	Screen* screen;
	GameConf* gc;
	int velocidadStandard;
	int velX,velY;

public:
	map<string, Vivible*> mapaEnemigos;
	HandlerEnemigos(SDL_Renderer* renderer, Resolucion* resolucion, Screen* screen, GameConf* gc);
	virtual ~HandlerEnemigos();
	void mover(string id, char event);
	void createEnemigo(string id, string type, int posX, int posY);
	void hacerVivir();
	void matar(string id);
	void deleteEnemys();
};
#endif /* SRC_GAME_CONTROLLER_HANDLERENEMIGOS_H_ */
