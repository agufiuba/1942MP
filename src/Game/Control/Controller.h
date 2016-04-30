/*
 * Controller.h
 *
 *  Created on: 30 de abr. de 2016
 *      Author: keynaka
 */

#ifndef SRC_CONTROL_CONTROLLER_H_
#define SRC_CONTROL_CONTROLLER_H_

#include "SDL2/SDL.h"
#include "SDL2/SDL_image.h"
#include "../Model/Avion.h"

#include <iostream>
#include <string>

class Controller {

private:
	Vivible* obj;
	int velX,velY;
	const int distanciaRecorrida = 4;

public:
	Controller(Vivible* unObj);
	~Controller();
	void press(SDL_Event *event);
	void hacerVivir();
};



#endif /* SRC_CONTROL_CONTROLLER_H_ */
