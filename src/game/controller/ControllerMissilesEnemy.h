/*
 * ControllerMissilesEnemy.h
 *
 *  Created on: 15 de jun. de 2016
 *      Author: keynaka
 */

#ifndef SRC_GAME_CONTROLLER_CONTROLLERMISSILESENEMY_H_
#define SRC_GAME_CONTROLLER_CONTROLLERMISSILESENEMY_H_

#include "../model/CompositeVivibles.h"
#include "SDL2/SDL.h"
#include "../model/Avion.h"
#include "../model/Misil.h"
#include "../model/MisilDirigido.h"
#include "../model/Resolucion.h"
#include "../../xml/conf/MisilConf.h"

#include <iostream>

using namespace std;

class ControllerMissilesEnemy{
	public:
		ControllerMissilesEnemy(MisilConf* config, SDL_Renderer* renderer);
		virtual ~ControllerMissilesEnemy();
		void press(SDL_Event *event);
		void hacerVivir();
		void crearNuevoMisilEnPosicion(int x, int y, Resolucion* resolucion, MisilConf* config, int posicionApuntadoX, int posicionApuntadoY);

		void setAmetralladora();
		void setBalaNormal();
		CompositeVivibles* getVivibles();
	private:
		SDL_Renderer* rendererMisil;
		int distanciaDeDesplazamiento = 25;//Todo sacarlo del hardcodeo
		CompositeVivibles* vivibles;
		bool ametralladora;
};

#endif /* SRC_GAME_CONTROLLER_CONTROLLERMISSILESENEMY_H_ */
