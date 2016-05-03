/*
 * Controller.h
 *
 *  Created on: 30 de abr. de 2016
 *      Author: keynaka
 */

#ifndef SRC_CONTROL_CONTROLLER_H_
#define SRC_CONTROL_CONTROLLER_H_

#include "SDL2/SDL.h"
#include "../Model/Avion.h"
#include "ControllerMissiles.h"

#include <iostream>
#include <string>

class Controller {

private:
	Vivible* obj;
	int velX,velY;
	const int distanciaDeDesplazamiento = 4;
	ControllerMissiles* controlDeMisiles;

public:
	Controller(Vivible* unObj, SDL_Renderer* &renderer);
	~Controller();
	void press(SDL_Event *event);
	void hacerVivir();
};

#endif /* SRC_CONTROL_CONTROLLER_H_ */
