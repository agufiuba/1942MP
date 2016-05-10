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
#include <vector>
#include "../model/Vivible.h"
#include "../model/Posicion.h"
#include "Texture.h"

class Isla : public Vivible {
private:
	const char* DIR_ISLA_1 = "src/game/images/islas/isla1.bmp";
	const char* DIR_ISLA_2 = "src/game/images/islas/isla2.bmp";
	const char* DIR_ISLA_3 = "src/game/images/islas/isla3-chica.bmp";
	const char* DIR_ISLA_4 = "src/game/images/islas/isla4-chica.bmp";
	const char* PORTAAVION = "src/game/images/porta1.bmp";
	Texture* islaTxt;
	Posicion* posicion;
	void inicializar();

public:
	Isla(SDL_Renderer* render, Posicion* posicion, int islaNumber);
	virtual ~Isla();
	void vivir(int x, int y);


	int getAnchoFondo();
	int getLargoFondo();
	int getAncho();
	int getLargo();
	int getX();
	int getY();

	void inicializoVueltereta();

	bool aunVive();
};

#endif /* ISLA_H_ */
