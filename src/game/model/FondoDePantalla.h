/*
 * FondoDePantalla.h
 *
 */

#ifndef FONDODEPANTALLA_H_
#define FONDODEPANTALLA_H_

#include <SDL2/SDL.h>
#include <iostream>
#include <string>
#include "../examples/libs/Texture.h"

class FondoDePantalla {

private:
	SDL_Window* window = NULL;
	Texture* fondoDePantalla = NULL;
	SDL_Rect posicion;
	SDL_Event evento;
	SDL_Renderer* gRenderer = NULL;

	bool inicioCorrectamente = true;
	bool isFullScreen = true;
	bool quit = false;

	int SCREEN_WIDTH = 1366;
	int SCREEN_HEIGHT = 768;
	const int FRAMES_PER_SECOND = 60;

	const char* DIR_FONDO_PANTALLA = "src/game/images/fondoPantalla-1800.bmp";
	const char* WINDOW_TITLE = "1942 MultiPlayer";

	void printErrorSDL(string error);
	void actualizarFondoDePantalla(int x, int y);
	int moverFondoDePantalla(int y);
	void aplicarFPS(Uint32 start);

public:
	FondoDePantalla();
	~FondoDePantalla();
	void run();

};

#endif /* FONDODEPANTALLA_H_ */
