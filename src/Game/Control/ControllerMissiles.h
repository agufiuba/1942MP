/*
 * ControllerAutomatic.h
 *
 *  Created on: 30 de abr. de 2016
 *      Author: ramon
 */

#ifndef SRC_GAME_CONTROL_CONTROLLERMISSILES_H_
#define SRC_GAME_CONTROL_CONTROLLERMISSILES_H_

#include "../Model/CompositeVivibles.h"
#include "SDL2/SDL.h"
#include "../Model/Avion.h"

#include <iostream>

using namespace std;

class ControllerMissiles{
	public:
		ControllerMissiles(Avion* &avion,SDL_Renderer* &renderer);
		virtual ~ControllerMissiles();
		void press(SDL_Event *event);
		void hacerVivir();
	private:
		CompositeVivibles* vivibles;
		Avion* avion = NULL;
		SDL_Renderer* rendererMisil = NULL;
		int distanciaDeDesplazamiento = distanciaDeDesplazamiento*1.5;//Todo sacarlo del hardcodeo

};

#endif /* SRC_GAME_CONTROL_CONTROLLERMISSILES_H_ */
