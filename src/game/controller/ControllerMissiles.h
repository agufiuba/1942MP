/*
 * ControllerAutomatic.h
 *
 *  Created on: 30 de abr. de 2016
 *      Author: ramon
 */

#ifndef SRC_GAME_CONTROL_CONTROLLERMISSILES_H_
#define SRC_GAME_CONTROL_CONTROLLERMISSILES_H_

#include "../model/CompositeVivibles.h"
#include "SDL2/SDL.h"
#include "../model/Avion.h"
#include "../model/Misil.h"
#include "../model/Resolucion.h"
#include "../../xml/conf/MisilConf.h"

#include <iostream>

using namespace std;

class ControllerMissiles{
	public:
		ControllerMissiles(MisilConf* config, SDL_Renderer* renderer);
		virtual ~ControllerMissiles();
		void press(SDL_Event *event);
		void hacerVivir();
		void crearNuevoMisilEnPosicion(int x, int y, Resolucion* resolucion, MisilConf* config);

		void setAmetralladora();
		void setBalaNormal();
		CompositeVivibles* vivibles;
	private:
		SDL_Renderer* rendererMisil;
		int distanciaDeDesplazamiento = 25;//Todo sacarlo del hardcodeo

		bool ametralladora;
};

#endif /* SRC_GAME_CONTROL_CONTROLLERMISSILES_H_ */
