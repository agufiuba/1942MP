/*
 * Isla.h
 *
 *  Created on: 03/05/2016
 *      Author: gonzalo
 */

#ifndef ISLA_H_
#define ISLA_H_

#include <SDL2/SDL.h>
#include <iostream>
#include "../model/Vivible.h"
#include "../model/Posicion.h"
#include "Texture.h"

class Isla : public Vivible {
private:
	const char* DIR_ISLA_1 = "src/game/images/isla1.bmp";
	SDL_Renderer* render;
	Texture* islaTxt;
	Posicion* posicion;

	void inicializar();

public:
	Isla(Posicion* posicion, SDL_Renderer* render);
	virtual ~Isla();
	void vivir(int x, int y);

};

#endif /* ISLA_H_ */
