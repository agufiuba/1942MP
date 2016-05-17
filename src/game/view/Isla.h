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
#include "../../xml/conf/SpriteConf.h"
#include <string>

class Isla : public Vivible {
private:
	Texture* islaTxt;
	Posicion* posicion;
	void inicializar();
	string mapearIsla(string);
	string id;

public:
	Isla(SDL_Renderer* render, Posicion* posicion, SpriteConf* id);
	virtual ~Isla();
	void vivir(int x, int y);

	string getId();
	int getAnchoFondo();
	int getLargoFondo();
	int getAncho();
	int getLargo();
	int getX();
	int getY();
	void inicializoVueltereta();
	bool aunVive();
	void setPosicion(Posicion* posicion);
	Posicion* getPosicion();

};

#endif /* ISLA_H_ */
