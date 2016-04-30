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
#include "../model/Resolucion.h"

class FondoDePantalla {

private:
	SDL_Window* window = NULL;
	Texture* fondoDePantalla = NULL;
	SDL_Renderer* gRenderer = NULL;
	SDL_Rect posicion;
	SDL_Event evento;
	Resolucion* resolucion;

	bool inicioCorrectamente;
	bool isFullScreen = true;

	int SCREEN_WIDTH;
	int SCREEN_HEIGHT;
	int FRAMES_PER_SECOND = 60;

	const char* DIR_FONDO_PANTALLA = "src/game/images/fondoPantalla-1800.bmp";
	const char* WINDOW_TITLE = "1942 MultiPlayer";

	void inicializar();
	void setResolucion();
	void printErrorSDL(string error);
	void actualizarFondoDePantalla(int x, int y);
	int moverFondoDePantalla(int y);
	void aplicarFPS(Uint32 start);

public:
	FondoDePantalla();
	FondoDePantalla(int fps);
	FondoDePantalla(int width, int height);
	FondoDePantalla(int fps, int width, int height);
	~FondoDePantalla();
	void run();

};

#endif /* FONDODEPANTALLA_H_ */
