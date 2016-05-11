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
#include <string>

class Isla : public Vivible {
private:
	string DIR_ISLA_1 = "src/game/images/islas/isla1.bmp";
	string DIR_ISLA_2 = "src/game/images/islas/isla2.bmp";
	string DIR_ISLA_3 = "src/game/images/islas/isla3-chica.bmp";
	string DIR_ISLA_4 = "src/game/images/islas/isla4-chica.bmp";
	string PORTAAVION = "src/game/images/portaviones/porta1.bmp";
	Texture* islaTxt;
	Posicion* posicion;
	void inicializar();
	string mapearIsla(string);

public:
	Isla(SDL_Renderer* render, Posicion* posicion, string id);
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
