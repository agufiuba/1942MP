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
#include <string>
#include "Texture.h"
#include "../model/Vivible.h"
#include "../model/Posicion.h"
#include "../view/Screen.h"
#include "../../xml/conf/SpriteConf.h"
#include "../../libs/xm_sdl/XM_SDL.h"

class Isla : public Vivible {
private:
	Texture* islaTxt;
	Posicion* posicion;
	string id;
	Screen* screenIsla;
	XM_SDL* sdl;

public:
	Isla(string index, Posicion* posicion, SpriteConf* id, XM_SDL* sdl);
	virtual ~Isla();
	void vivir();
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
