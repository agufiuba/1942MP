/*
 * PowerUpView.h
 *
 *  Created on: 24 de may. de 2016
 *      Author: keynaka
 */

#ifndef SRC_GAME_VIEW_POWERUPVIEW_H_
#define SRC_GAME_VIEW_POWERUPVIEW_H_

#include "Texture.h"
#include <string>
#include <iostream>

class PowerUpView {
private:
	Texture *vistaPowerUpTexture = NULL;
	SDL_Renderer *rendererPowerUp;

	//string dir = "src/game/images/";

	int anchoVista;
	int largoVista;

public:
	PowerUpView(SDL_Renderer* renderer);
	~PowerUpView();

	bool cargarImagenDelPowerUp( string filename );

	int getAnchoVista();
	int getLargoVista();

	void mostrar(int x, int y);
};

#endif /* SRC_GAME_VIEW_POWERUPVIEW_H_ */
